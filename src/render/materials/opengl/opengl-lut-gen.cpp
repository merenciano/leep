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

    vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
    {
        float a = roughness*roughness;
        
        float phi = 2.0 * kPI * Xi.x;
        float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
        float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
        
        // from spherical coordinates to cartesian coordinates - halfway vector
        vec3 H;
        H.x = cos(phi) * sinTheta;
        H.y = sin(phi) * sinTheta;
        H.z = cosTheta;
        
        // from tangent-space H vector to world-space sample vector
        vec3 up          = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
        vec3 tangent   = normalize(cross(up, N));
        vec3 bitangent = cross(N, tangent);
        
        vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
        return normalize(sampleVec);
    }

    float GeometrySchlickGGX(float NdotV, float roughness)
    {
        // note that we use a different k for IBL
        float a = roughness;
        float k = (a * a) / 2.0;

        float nom   = NdotV;
        float denom = NdotV * (1.0 - k) + k;

        return nom / denom;
    }

    float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
    {
        float NdotV = max(dot(N, V), 0.0);
        float NdotL = max(dot(N, L), 0.0);
        float ggx2 = GeometrySchlickGGX(NdotV, roughness);
        float ggx1 = GeometrySchlickGGX(NdotL, roughness);

        return ggx1 * ggx2;
    }

    vec2 IntegrateBRDF(float NdotV, float roughness)
    {
        vec3 V;
        V.x = sqrt(1.0 - NdotV*NdotV);
        V.y = 0.0;
        V.z = NdotV;

        float A = 0.0;
        float B = 0.0;

        vec3 N = vec3(0.0, 0.0, 1.0);

        for(uint i = 0u; i < kSampleCount; ++i)
        {
            vec2 Xi = Hammersley(i);
            vec3 H  = ImportanceSampleGGX(Xi, N, roughness);
            vec3 L  = normalize(2.0 * dot(V, H) * H - V);

            float NdotL = max(L.z, 0.0);
            float NdotH = max(H.z, 0.0);
            float VdotH = max(dot(V, H), 0.0);

            if(NdotL > 0.0)
            {
                float G = GeometrySmith(N, V, L, roughness);
                float G_Vis = (G * VdotH) / (NdotH * NdotV);
                float Fc = pow(1.0 - VdotH, 5.0);

                A += (1.0 - Fc) * G_Vis;
                B += Fc * G_Vis;
            }
        }
        A *= kInvSampleCount;
        B *= kInvSampleCount;
        return vec2(A, B);
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

    // Compute Van der Corput radical inverse
    // See: http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
    float radicalInverse_VdC(uint bits)
    {
        bits = (bits << 16u) | (bits >> 16u);
        bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
        bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
        bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
        bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
        return float(bits) * 2.3283064365386963e-10; // / 0x100000000
    }

    // Sample i-th point from Hammersley point set of NumSamples points total.
    vec2 sampleHammersley(uint i)
    {
        return vec2(i * kInvSampleCount, radicalInverse_VdC(i));
    }

    // Importance sample GGX normal distribution function for a fixed roughness value.
    // This returns normalized half-vector between Li & Lo.
    // For derivation see: http://blog.tobias-franke.eu/2014/03/30/notes_on_importance_sampling.html
    vec3 sampleGGX(float u1, float u2, float roughness)
    {
        float alpha = roughness * roughness;

        float cosTheta = sqrt((1.0 - u2) / (1.0 + (alpha*alpha - 1.0) * u2));
        float sinTheta = sqrt(1.0 - cosTheta*cosTheta); // Trig. identity
        float phi = kTwoPI * u1;

        // Convert to Cartesian upon return.
        return vec3(sinTheta * cos(phi), sinTheta * sin(phi), cosTheta);
    }

    // Single term for separable Schlick-GGX below.
    float gaSchlickG1(float cosTheta, float k)
    {
        return cosTheta / (cosTheta * (1.0 - k) + k);
    }

    // Schlick-GGX approximation of geometric attenuation function using Smith's method (IBL version).
    float gaSchlickGGX_IBL(float cosLi, float cosLo, float roughness)
    {
        float r = roughness;
        float k = (r * r) / 2.0; // Epic suggests using this roughness remapping for IBL lighting.
        return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
    }

    void main() {
        // Get integration parameters.
	    float cosLo = uv.x;
	    float roughness = uv.y;
        // Make sure viewing angle is non-zero to avoid divisions by zero (and subsequently NaNs).
	    cosLo = max(cosLo, kEpsilon);

	    // Derive tangent-space viewing vector from angle to normal (pointing towards +Z in this reference frame).
	    vec3 Lo = vec3(sqrt(1.0 - cosLo*cosLo), 0.0, cosLo);

        // We will now pre-integrate Cook-Torrance BRDF for a solid white environment and save results into a 2D LUT.
        // DFG1 & DFG2 are terms of split-sum approximation of the reflectance integral.
        // For derivation see: "Moving Frostbite to Physically Based Rendering 3.0", SIGGRAPH 2014, section 4.9.2.
        float DFG1 = 0;
        float DFG2 = 0;

        for(uint i=0u; i < kSampleCount; ++i) {
            vec2 u  = sampleHammersley(i);

            // Sample directly in tangent/shading space since we don't care about reference frame as long as it's consistent.
            vec3 Lh = sampleGGX(u.x, u.y, roughness);

            // Compute incident direction (Li) by reflecting viewing direction (Lo) around half-vector (Lh).
            vec3 Li = 2.0 * dot(Lo, Lh) * Lh - Lo;

            float cosLi   = Li.z;
            float cosLh   = Lh.z;
            float cosLoLh = max(dot(Lo, Lh), 0.0);

            if(cosLi > 0.0) {
                float G  = gaSchlickGGX_IBL(cosLi, cosLo, roughness);
                float Gv = G * cosLoLh / (cosLh * cosLo);
                float Fc = pow(1.0 - cosLoLh, 5);

                DFG1 += (1 - Fc) * Gv;
                DFG2 += Fc * Gv;
            }
        }

        FragColor = vec2(DFG1 * kInvSampleCount, DFG2 * kInvSampleCount);
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