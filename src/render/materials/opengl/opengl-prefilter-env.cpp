#include "render/materials/prefilter-env.h"
#include "core/common-defs.h"
#include "core/manager.h"
#include "core/logger.h"
#include "render/renderer.h"
#include "render/commands/create-cubemap.h"

#include <glad/glad.h>

static const char* kPrefilterEnvVertex = R"(
    #version 330 core
    layout (location = 0) in vec3 a_position;

    out vec3 position;

    uniform mat4 u_vp;

    void main()
    {
        position = a_position;
        gl_Position = u_vp * vec4(position, 1.0);
    }
)";

static const char* kPrefilterEnvFragment = R"(
    #version 330 core

    uniform samplerCube u_environment_map;
    uniform float u_roughness;

    const float PI = 3.14159265359;
    const uint SAMPLE_COUNT = 1024u;

    out vec4 FragColor;
    in vec3 position;

    float RadicalInverse_VdC(uint bits) 
    {
        bits = (bits << 16u) | (bits >> 16u);
        bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
        bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
        bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
        bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
        return float(bits) * 2.3283064365386963e-10; // / 0x100000000
    }

    vec2 Hammersley(uint i, uint N)
    {
        return vec2(float(i)/float(N), RadicalInverse_VdC(i));
    }  

    vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
    {
        float a = roughness*roughness;
        
        float phi = 2.0 * PI * Xi.x;
        float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
        float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
        
        // from spherical coordinates to cartesian coordinates
        vec3 H;
        H.x = cos(phi) * sinTheta;
        H.y = sin(phi) * sinTheta;
        H.z = cosTheta;
        
        // from tangent-space vector to world-space sample vector
        vec3 up        = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
        vec3 tangent   = normalize(cross(up, N));
        vec3 bitangent = cross(N, tangent);
        
        vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
        return normalize(sampleVec);
    }

    void main()
    {
        vec3 N = normalize(position);    
        vec3 R = N;
        vec3 V = R;

        float totalWeight = 0.0;   
        vec3 prefilteredColor = vec3(0.0);     
        for(uint i = 0u; i < SAMPLE_COUNT; ++i)
        {
            vec2 Xi = Hammersley(i, SAMPLE_COUNT);
            vec3 H  = ImportanceSampleGGX(Xi, N, u_roughness);
            vec3 L  = normalize(2.0 * dot(V, H) * H - V);

            float NdotL = max(dot(N, L), 0.0);
            if(NdotL > 0.0)
            {
                prefilteredColor += texture(u_environment_map, L).rgb * NdotL;
                totalWeight      += NdotL;
            }
        }
        prefilteredColor = prefilteredColor / totalWeight;

        FragColor = vec4(prefilteredColor, 1.0);
    }
)";

namespace leep {

void PrefilterEnv::init()
{
    GLint err;
    GLchar output_log[512];
    //  Create and compile vertex shader and print if compilation errors
    GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &kPrefilterEnvVertex, nullptr);
    glCompileShader(vert_shader);
    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &err);
    if (!err)
    {
        glGetShaderInfoLog(vert_shader, 512, nullptr, output_log);
        LEEP_CORE_ERROR(
            "PrefilterEnv vertex shader compilation failed:\n{0}\n", output_log);
    }
    //  Create and compile fragment shader and print if compilation errors
    GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &kPrefilterEnvFragment, nullptr);
    glCompileShader(frag_shader);
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &err);
    if (!err)
    {
        glGetShaderInfoLog(frag_shader, 512, nullptr, output_log);
        LEEP_CORE_ERROR(
            "PrefilterEnv fragment shader compilation failed:\n{0}\n", output_log);
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
        LEEP_CORE_ERROR("PrefilterEnv program error:\n{0}\n", output_log);
    }

    internal_id_ = (uint32_t)program;
}

void PrefilterEnv::useMaterialData(const Material& material) const
{
    GLenum err = glGetError();
    Renderer& r = GM.renderer();
    LEEP_ASSERT(material.type() == MaterialType::MT_PREFILTER_ENV,
        "Wrong material type");

    // Load textures
    int32_t env_id = material.albedo().handle();
    LEEP_CORE_ASSERT(r.textures_[env_id].cpu_version_ > 0, "Invalid texture");
    glUseProgram(internal_id_);

    if (env_id != CommonDefs::UNINIT_HANDLE)
    {
        LEEP_CHECK_RESOURCE(material.albedo());
        LEEP_ASSERT(r.textures_[env_id].cpu_version_ != CommonDefs::DELETED_GPU_RESOURCE, "Texture released");
        if (r.textures_[env_id].gpu_version_ == 0)
        {
            CreateCubemap().set_texture(material.albedo()).executeCommand();
        }
    }

    err = glGetError();
    GLuint u_loc = glGetUniformLocation(internal_id_, "u_environment_map");
    glUniform1i(u_loc, r.textures_[env_id].texture_unit_);
    u_loc = glGetUniformLocation(internal_id_, "u_roughness");
    glUniform1f(u_loc, material.data().pref_.roughness_);
    u_loc = glGetUniformLocation(internal_id_, "u_vp");
    glUniformMatrix4fv(u_loc, 1, false, (const GLfloat*)&(material.data()));
    err = glGetError();
}
}