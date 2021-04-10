#include "render/materials/lut-gen.h"

#include "core/logger.h"
#include "core/common-defs.h"
#include "core/manager.h"
#include "render/renderer.h"
#include "render/commands/create-texture.h"

#include <glad/glad.h>

static const char* kLutGenVertex = R"(
    #version 330 core 

    layout (location = 0) in vec3 a_position;
    layout (location = 2) in vec2 a_uv;

    out vec2 uv;

    void main() {
        uv = a_uv;
        gl_Position = vec4(a_position, 1.0);
    }
)";

static const char* kLutGenFragment = R"(
    #version 330 core 

    in vec2 uv;
    out vec2 FragColor;

    const float kPI = 3.14159265359;
    const uint kSampleCount = 1024u;
    const float kInvSampleCount = 1.0 / float(kSampleCount);

    // Van der Corput radical inverse
    float RadicalInverse_VdC(uint bits) 
    {
        bits = (bits << 16u) | (bits >> 16u);
        bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
        bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
        bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
        bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
        return float(bits) * 2.3283064365386963e-10; // / 0x100000000
    }

    vec2 Hammersley(uint i)
    {
        return vec2(i * kInvSampleCount, RadicalInverse_VdC(i));
    }

    vec3 ImportanceSampleGGX(vec2 xi, vec3 n, float roughness)
    {
        float a = roughness*roughness;
        
        float phi = 2.0 * kPI * xi.x;
        float cos_theta = sqrt((1.0 - xi.y) / (1.0 + (a*a - 1.0) * xi.y));
        float sin_theta = sqrt(1.0 - cos_theta*cos_theta);
        
        // from spherical coordinates to cartesian coordinates - halfway vector
        vec3 h;
        h.x = cos(phi) * sin_theta;
        h.y = sin(phi) * sin_theta;
        h.z = cos_theta;
        
        // from tangent-space h vector to world-space sample vector
        vec3 up = abs(n.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
        vec3 tangent = normalize(cross(up, n));
        vec3 bitangent = cross(n, tangent);
        
        vec3 sample = tangent * h.x + bitangent * h.y + n * h.z;
        return normalize(sample);
    }

    float GeometrySchlickGGX(float NoV, float roughness)
    {
        float a = roughness;
        float k = (a * a) / 2.0;

        float nom   = NoV;
        float denom = NoV * (1.0 - k) + k;

        return nom / denom;
    }

    float GeometrySmith(vec3 n, vec3 v, vec3 l, float roughness)
    {
        float NoV = max(dot(n, v), 0.0);
        float NoL = max(dot(n, l), 0.0);
        float ggx2 = GeometrySchlickGGX(NoV, roughness);
        float ggx1 = GeometrySchlickGGX(NoL, roughness);

        return ggx1 * ggx2;
    }

    vec2 IntegrateBRDF(float NoV, float roughness)
    {
        vec3 v;
        v.x = sqrt(1.0 - NoV*NoV);
        v.y = 0.0;
        v.z = NoV;

        float a = 0.0;
        float b = 0.0;

        vec3 n = vec3(0.0, 0.0, 1.0);

        for(uint i = 0u; i < kSampleCount; ++i)
        {
            vec2 xi = Hammersley(i);
            vec3 h  = ImportanceSampleGGX(xi, n, roughness);
            vec3 l  = normalize(2.0 * dot(v, h) * h - v);

            float NoL = max(l.z, 0.0);
            float NoH = max(h.z, 0.0);
            float VoH = max(dot(v, h), 0.0);

            if(NoL > 0.0)
            {
                float g = GeometrySmith(n, v, l, roughness);
                float g_vis = (g * VoH) / (NoH * NoV);
                float fc = pow(1.0 - VoH, 5.0);

                a += (1.0 - fc) * g_vis;
                b += fc * g_vis;
            }
        }
        a *= kInvSampleCount;
        b *= kInvSampleCount;
        return vec2(a, b);
    }

    void main() {
        vec2 integratedBRDF = IntegrateBRDF(uv.x, uv.y);
        FragColor = integratedBRDF;
    }
)";

static const char* kLutGen2Fragment = R"(
    #version 330 core 

    in vec2 uv;
    out vec2 FragColor;

    const float kTwoPI = 2.0 * 3.14159265359;
    const uint  kSampleCount = 1024u;
    const float kInvSampleCount = 1.0 / float(kSampleCount);
    const float kEpsilon = 0.001; // This program needs larger eps.

    float RadicalInverse_VdC(uint bits)
    {
        bits = (bits << 16u) | (bits >> 16u);
        bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
        bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
        bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
        bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
        return float(bits) * 2.3283064365386963e-10; // / 0x100000000
    }

    // Sample i-th point from Hammersley point set of NumSamples points total.
    vec2 SampleHammersley(uint i)
    {
        return vec2(i * kInvSampleCount, RadicalInverse_VdC(i));
    }

    vec3 SampleGGX(float u1, float u2, float roughness)
    {
        float alpha = roughness * roughness;

        float cos_theta = sqrt((1.0 - u2) / (1.0 + (alpha*alpha - 1.0) * u2));
        float sin_theta = sqrt(1.0 - cos_theta*cos_theta);
        float phi = kTwoPI * u1;

        // Convert to Cartesian upon return.
        return vec3(sin_theta * cos(phi), sin_theta * sin(phi), cos_theta);
    }

    // Single term for separable Schlick-GGX below.
    float GASchlickG1(float cos_theta, float k)
    {
        return cos_theta / (cos_theta * (1.0 - k) + k);
    }

    // Schlick-GGX approximation of geometric attenuation function using Smith's method (IBL version).
    float GASchlickGGX_IBL(float cos_li, float cos_lo, float roughness)
    {
        float r = roughness;
        float k = (r * r) / 2.0;
        return GASchlickG1(cos_li, k) * GASchlickG1(cos_lo, k);
    }

    void main() {
	    float roughness = uv.y;
	    float cos_lo = max(uv.x, kEpsilon);

        // viewing direction
	    vec3 lo = vec3(sqrt(1.0 - cos_lo*cos_lo), 0.0, cos_lo);

        float dfg1 = 0;
        float dfg2 = 0;

        for(uint i=0u; i < kSampleCount; ++i) {
            vec2 u = SampleHammersley(i);
            // half-vector
            vec3 lh = SampleGGX(u.x, u.y, roughness);

            // Incident direction
            vec3 li = 2.0 * dot(lo, lh) * lh - lo;

            float cos_li = li.z;
            float cos_lh = lh.z;
            float cos_lo_lh = max(dot(lo, lh), 0.0);

            if(cos_li > 0.0)
            {
                float g  = GASchlickGGX_IBL(cos_li, cos_lo, roughness);
                float gv = g * cos_lo_lh / (cos_lh * cos_lo);
                float fc = pow(1.0 - cos_lo_lh, 5);

                dfg1 += (1 - fc) * gv;
                dfg2 += fc * gv;
            }
        }

        FragColor = vec2(dfg1 * kInvSampleCount, dfg2 * kInvSampleCount);
    }
)";

namespace leep
{
    void LutGen::init()
    {
        GLint err;
        GLchar output_log[512];
        //  Create and compile vertex shader and print if compilation errors
        GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vert_shader, 1, &kLutGenVertex, nullptr);
        glCompileShader(vert_shader);
        glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &err);
        if (!err)
        {
            glGetShaderInfoLog(vert_shader, 512, nullptr, output_log);
            LEEP_CORE_ERROR("Lut tex generator vertex shader compilation failed:\n{0}\n", output_log);
        }
        //  Create and compile fragment shader and print if compilation errors
        GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag_shader, 1, &kLutGen2Fragment, nullptr);
        glCompileShader(frag_shader);
        glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &err);
        if (!err)
        {
            glGetShaderInfoLog(frag_shader, 512, nullptr, output_log);
            LEEP_CORE_ERROR("Lut tex generator fragment shader compilation failed:\n{0}\n", output_log);
        }
        //  Create the program with both shaders
        GLuint program = glCreateProgram();
        glAttachShader(program, vert_shader);
        glAttachShader(program, frag_shader);
        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &err);
        if (!err)
        {
            glGetProgramInfoLog(program, 512, nullptr, output_log);
            LEEP_CORE_ERROR("Lut tex generatorprogram error:\n{0}\n", output_log);
        }

        internal_id_ = (uint32_t)program;
    }

    void LutGen::useMaterialData(const Material &material) const
    {
        LEEP_CORE_ASSERT(material.type() == MaterialType::MT_LUT_GEN,
            "Wrong material type");
        
        // No uniforms
        glUseProgram(internal_id_);
    }
}