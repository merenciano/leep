#include "render/materials/pbr.h"
#include "core/common-defs.h"
#include "core/manager.h"
#include "core/logger.h"
#include "render/renderer.h"
#include "render/commands/create-texture.h"

#include <glad/glad.h>

static const char* kPbrVertex = R"(
    #version 330 core

    #define TILING_X u_entity_data[5].y
    #define TILING_Y u_entity_data[5].z

    uniform vec4 u_entity_data[7];
    uniform vec4 u_scene_data[6];

    layout(location=0) in vec3 a_position;
    layout(location=1) in vec3 a_normal;
    layout(location=2) in vec3 a_tangent;
    layout(location=3) in vec3 a_bitangent;
    layout(location=4) in vec2 a_uv;

    out Vertex
    {
	    mat3 tbn;
	    vec3 position;
	    vec2 uv;
    } v_out;

    void main()
    {
        mat4 model = mat4(u_entity_data[0], u_entity_data[1],
                          u_entity_data[2], u_entity_data[3]);
        mat4 vp = mat4(u_scene_data[0], u_scene_data[1],
                       u_scene_data[2], u_scene_data[3]);

	    v_out.position = vec3(model * vec4(a_position, 1.0));
	    v_out.uv = vec2(a_uv.x * TILING_X, a_uv.y * TILING_Y);

	    // Pass tangent space basis vectors (for normal mapping).
	    v_out.tbn = mat3(model) * mat3(normalize(a_tangent), normalize(a_bitangent), normalize(a_normal));

	    gl_Position = vp * model * vec4(a_position, 1.0);
    }
)";

static const char* kPbrFragment = R"(
    #version 330 core

    #define COLOR				u_entity_data[4].xyz
    #define USE_ALBEDO_MAP		u_entity_data[4].w
    #define USE_PBR_MAPS		u_entity_data[5].x
    #define ROUGHNESS			u_entity_data[6].x
    #define METALLIC			u_entity_data[6].y
    #define NORMAL_MAP_INTENSITY u_entity_data[6].z

    #define LIGHT_DIRECTION     u_scene_data[5].xyz
    #define LIGHT_INTENSITY     u_scene_data[5].w
    #define CAMERA_POSITION     u_scene_data[4].xyz

    const float kPI = 3.14159265359;
    const float kEpsilon = 0.00001;
    // Constant normal incidence Fresnel factor for all dielectrics.
    const vec3 kFdielectric = vec3(0.04);
    const int kMaxPrefilterLod = 5;

    in Vertex
    {
	    mat3 tbn;
	    vec3 position;
	    vec2 uv;
    } v_in;

    out vec4 FragColor;

    uniform vec4 u_entity_data[7];
    uniform vec4 u_scene_data[6];
    uniform sampler2D u_albedo;
    uniform sampler2D u_metallic;
    uniform sampler2D u_roughness;
    uniform sampler2D u_normal;
    uniform samplerCube u_irradiance_map;
    uniform samplerCube u_prefilter_map;
    uniform sampler2D u_lut;

    // GGX/Towbridge-Reitz normal distribution function.
    // Uses Disney's reparametrization of alpha = roughness^2.
    float NDFGGX(float cos_lh, float roughness)
    {
	    float a = roughness * roughness;

	    float denom = (cos_lh * cos_lh) * ((a * a) - 1.0) + 1.0;
	    return (a * a) / (kPI * denom * denom);
    }

    // Single term for separable Schlick-GGX below.
    float GASchlickG1(float cos_theta, float k)
    {
	    return cos_theta / (cos_theta * (1.0 - k) + k);
    }

    // Schlick-GGX Geometric Attenuation
    float GASchlickGGX(float cos_li, float cos_lo, float roughness)
    {
	    float r = roughness + 1.0;
	    float k = (r * r) / 8.0;
	    return GASchlickG1(cos_li, k) * GASchlickG1(cos_lo, k);
    }

    // Shlick's Fresnel
    vec3 FresnelSchlick(vec3 F0, float cos_theta)
    {
	    return F0 + (vec3(1.0) - F0) * pow(1.0 - cos_theta, 5.0);
    }

    void main()
    {
	    // Sample input textures to get shading model params.
	    vec3 albedo = texture(u_albedo, v_in.uv).rgb;
        albedo = mix(COLOR, albedo, USE_ALBEDO_MAP);
	    float metalness = texture(u_metallic, v_in.uv).r;
        metalness = mix(METALLIC, metalness, USE_PBR_MAPS);
	    float roughness = texture(u_roughness, v_in.uv).r;
        roughness = mix(ROUGHNESS, roughness, USE_PBR_MAPS);

	    // Outgoing light direction (vector from world-space fragment position to the "eye").
	    vec3 lo = normalize(CAMERA_POSITION - v_in.position);

	    // Get current fragment's normal and transform to world space.
	    vec3 normal = normalize(2.0 * texture(u_normal, v_in.uv).rgb - 1.0);
	    normal = normalize(v_in.tbn * normal);
        normal = mix(normalize(v_in.tbn[2]), clamp(normal, -1.0, 1.0), NORMAL_MAP_INTENSITY);
    
	    // Angle between surface normal and outgoing light direction.
	    float cos_lo = max(0.0, dot(normal, lo));
		
	    // Specular reflection vector.
	    vec3 lr = 2.0 * cos_lo * normal - lo;


	    // Fresnel reflectance at normal incidence (for metals use albedo color).
	    vec3 f0 = mix(kFdielectric, albedo, metalness);

	    // Direct lighting calculation for analytical lights.
	    vec3 direct_lighting;
        vec3 li = -LIGHT_DIRECTION;

        // Half-vector between Li and Lo.
        vec3 lh = normalize(li + lo);

        // Calculate angles between surface normal and various light vectors.
        float cos_li = max(0.0, dot(normal, li));
        float cos_lh = max(0.0, dot(normal, lh));

        vec3 f = FresnelSchlick(f0, max(0.0, dot(lh, lo)));
        float d = NDFGGX(cos_lh, roughness);
        float g = GASchlickGGX(cos_li, cos_lo, roughness);

        // Lambert diffuse BRDF.
        vec3 kd = mix(vec3(1.0) - f, vec3(0.0), metalness);
        vec3 diffuse_brdf = kd * albedo;

        // Cook-Torrance specular BRDF.
        vec3 specular_brdf = (f * d * g) / max(kEpsilon, 4.0 * cos_li * cos_lo);

        // Total contribution for this light.
        direct_lighting = (diffuse_brdf + specular_brdf) * LIGHT_INTENSITY * cos_li;

        // IBL
        vec3 irradiance = texture(u_irradiance_map, normal).rgb;
        vec3 f_ibl = FresnelSchlick(f0, cos_lo);
        vec3 kd_ibl = mix(vec3(1.0) - f_ibl, vec3(0.0), metalness);
        vec3 diffuse_ibl = kd_ibl * albedo * irradiance;

        vec3 specular_irradiance = textureLod(u_prefilter_map, lr, roughness * kMaxPrefilterLod).rgb;
        vec2 specular_brdf_ibl = texture(u_lut, vec2(cos_lo, roughness)).rg;
        vec3 specular_ibl = (f0 * specular_brdf_ibl.x + specular_brdf_ibl.y) * specular_irradiance;
        vec3 ambient_lighting = diffuse_ibl + specular_ibl;

	    FragColor = vec4(direct_lighting + ambient_lighting, 1.0);
    }
)";

namespace leep
{
    void Pbr::init()
    {
        GLint err;
        GLchar output_log[512];
        //  Create and compile vertex shader and print if compilation errors
        GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vert_shader, 1, &kPbrVertex, nullptr);
        glCompileShader(vert_shader);
        glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &err);
        if (!err)
        {
            glGetShaderInfoLog(vert_shader, 512, nullptr, output_log);
            LEEP_CORE_ERROR(
				"PBR vertex shader compilation failed:\n{0}\n", output_log);
        }
        //  Create and compile fragment shader and print if compilation errors
        GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag_shader, 1, &kPbrFragment, nullptr);
        glCompileShader(frag_shader);
        glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &err);
        if (!err)
        {
            glGetShaderInfoLog(frag_shader, 512, nullptr, output_log);
            LEEP_CORE_ERROR(
                "PBR fragment shader compilation failed:\n{0}\n", output_log);
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
            LEEP_CORE_ERROR("PBR program error:\n{0}\n", output_log);
        }

        internal_id_ = (uint32_t)program;
    }

    void Pbr::useMaterialData(const Material& material) const
    {
        GLuint u_loc;
        Renderer& r = GM.renderer();
        LEEP_ASSERT(material.type() == MaterialType::MT_PBR,
			"Wrong material type");

        // Load textures
        int32_t a_id = material.albedo().handle();
        if (a_id != CommonDefs::UNINIT_HANDLE)
        {
            LEEP_ASSERT(a_id != -1, "Texture not created");
            LEEP_ASSERT(r.textures_[a_id].cpu_version_ != -1, "Texture released");
            if (r.textures_[a_id].gpu_version_ == 0)
            {
                CreateTexture()
                    .set_texture(material.albedo()).executeCommand();
            }
            u_loc = glGetUniformLocation(internal_id_, "u_albedo");
            glUniform1i(u_loc, r.textures_[a_id].texture_unit_);
        }

        int32_t m_id = material.metallic().handle();
        if (m_id != CommonDefs::UNINIT_HANDLE)
        {
            LEEP_ASSERT(m_id != -1, "Texture not created");
            LEEP_ASSERT(r.textures_[m_id].cpu_version_ != -1, "Texture released");
            if (r.textures_[m_id].gpu_version_ == 0)
            {
                CreateTexture()
                    .set_texture(material.metallic()).executeCommand();
            }
            u_loc = glGetUniformLocation(internal_id_, "u_metallic");
            glUniform1i(u_loc, r.textures_[m_id].texture_unit_);
        }

        int32_t r_id = material.roughness().handle();
        if (r_id != CommonDefs::UNINIT_HANDLE)
        {
            LEEP_ASSERT(r_id != -1, "Texture not created");
            LEEP_ASSERT(r.textures_[r_id].cpu_version_!= -1, "Texture released");
            if (r.textures_[r_id].gpu_version_ == 0)
            {
                CreateTexture()
                    .set_texture(material.roughness()).executeCommand();
            }
            u_loc = glGetUniformLocation(internal_id_, "u_roughness");
            glUniform1i(u_loc, r.textures_[r_id].texture_unit_);
        }

        int32_t n_id = material.normal().handle();
        if (n_id != CommonDefs::UNINIT_HANDLE)
        {
            LEEP_ASSERT(n_id != -1, "Texture not created");
            LEEP_ASSERT(r.textures_[n_id].cpu_version_ != -1, "Texture released");
            if (r.textures_[n_id].gpu_version_ == 0)
            {
                CreateTexture()
                    .set_texture(material.normal()).executeCommand();
            }

            u_loc = glGetUniformLocation(internal_id_, "u_normal");
            glUniform1i(u_loc, r.textures_[n_id].texture_unit_);
        }
        
        u_loc = glGetUniformLocation(internal_id_, "u_entity_data");
        glUniform4fv(u_loc, sizeof(PbrData) / sizeof(float) / 4,
					 (const GLfloat*)&(material.data()));
    }
}
