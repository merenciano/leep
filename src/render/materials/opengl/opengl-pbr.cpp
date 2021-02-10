#include "render/materials/pbr.h"
#include "core/common-defs.h"
#include "core/manager.h"
#include "core/logger.h"
#include "render/renderer.h"
#include "render/commands/create-texture.h"

#include <glad/glad.h>

static const char* kPbrVertex = R"(
    #version 330 core 

    uniform vec4 u_entity_data[7];
    uniform vec4 u_scene_data[6];

    layout (location = 0) in vec3 a_position;
    layout (location = 1) in vec3 a_normal;
    layout (location = 2) in vec2 a_uv;

	out vec4 world_position;
	out vec3 normal;
	out vec3 camera_position;
	out vec3 light_direction;
	out float light_intensity;
    out vec2 uv;

    void main() {
        mat4 world = mat4(u_entity_data[0], u_entity_data[1], u_entity_data[2], u_entity_data[3]);
        mat4 vp = mat4(u_scene_data[0], u_scene_data[1], u_scene_data[2], u_scene_data[3]);
        world_position = world * vec4(a_position, 1.0);
		normal = normalize(mat3(world) * a_normal);
        uv = a_uv;
		light_direction = u_scene_data[5].xyz;
		light_intensity = u_scene_data[5].w;
		camera_position = vec3(u_scene_data[4]);
        gl_Position = vp * world_position;
    }
)";

static const char* kPbrFragment = R"(
    #version 330 core 

	#define COLOR				u_entity_data[4].xyz
	#define USE_ALBEDO_MAP		u_entity_data[4].w
	#define TILING_X 			u_entity_data[5].y
	#define TILING_Y 			u_entity_data[5].z

	#define ROUGHNESS			u_entity_data[6].x
	#define METALLIC			u_entity_data[6].y
	#define REFLECTANCE			u_entity_data[6].z

	#define PI 3.14159265359

    uniform vec4 u_entity_data[7];
    uniform sampler2D u_albedo;

	in vec4 world_position;
	in vec3 normal;
	in vec3 camera_position;
	in vec3 light_direction;
	in float light_intensity;
    in vec2 uv;
    out vec4 FragColor;

	struct Material
	{
		vec3  albedo;
		float roughness;
		float metallic;
		float reflectance;
	};

	// Normal distribution function (specular D)
	float D_GGX(float NoH, float roughness) {
		float a = NoH * roughness;
		float k = roughness / ((1.0 - NoH * NoH) + a * a);
		return k * k * (1.0 / PI);
	}

    float V_SmithGGXCorrelated(float NoV, float NoL, float roughness) {
        float a2 = roughness * roughness;
        float lambdaV = NoL * sqrt((NoV - a2 * NoV) * NoV + a2);
        float lambdaL = NoV * sqrt((NoL - a2 * NoL) * NoL + a2);
        return 0.5 / (lambdaV + lambdaL);
    }

	// Visibility (specular V)
	float V_SmithGGXCorrelatedFast(float NoV, float NoL, float roughness) {
		float a = roughness;
		float GGXV = NoL * (NoV * (1.0 - a) + a);
		float GGXL = NoV * (NoL * (1.0 - a) + a);
		return 0.5 / (GGXV + GGXL); // TODO: Test with max(that, 0.01)
	}

	// Fresnel
	vec3 F_Schlick(float u, vec3 f0) {
		float f = pow(1.0 - u, 5.0);
		return f + f0 * (1.0 - f);
	}

	// Diffuse BRDF
	float Fd_Lambert() {
		return 1.0 / PI;
	}

	vec3 BRDF(Material mat, vec3 l, vec3 v) {
		vec3 n = normal;
		vec3 h = normalize(v + l);

		vec3 f0 = 0.16 * mat.reflectance * mat.reflectance * (1.0 - mat.metallic) + mat.albedo * mat.metallic;

		float NoV = abs(dot(n, v)) + 1e-5;
		float NoL = clamp(dot(n, l), 0.0, 1.0);
		float NoH = clamp(dot(n, h), 0.0, 1.0);
		float LoH = clamp(dot(l, h), 0.0, 1.0);

		// perceptually linear roughness to roughness
		float roughness = mat.roughness * mat.roughness;

		float D = D_GGX(NoH, roughness);
		vec3  F = F_Schlick(LoH, f0);
		float V = V_SmithGGXCorrelated(NoV, NoL, roughness);
		// specular BRDF
		vec3 Fr = (D * V) * F;

		// diffuse BRDF
		vec3 Fd = mat.albedo * Fd_Lambert();

		// apply lighting...
		return Fd + Fr;
	}

    void main() {
		Material material;
		material.roughness = ROUGHNESS;
		material.metallic = METALLIC;
		material.reflectance = REFLECTANCE;

		vec3 albedo = texture(u_albedo, vec2(uv.x * TILING_X, uv.y * TILING_Y)).rgb;
		material.albedo = (1.0 - USE_ALBEDO_MAP) * COLOR + USE_ALBEDO_MAP * albedo;
		material.albedo *= (1.0 - material.metallic);


		vec3 l = normalize(light_direction);
		float NoL = clamp(dot(normal, l), 0.0, 1.0);
		float illuminance = light_intensity * NoL;
		vec3 color = BRDF(material, l, normalize(camera_position - vec3(world_position)));
		vec3 ambient = material.albedo * 0.03;
        //gamma correction
        color = pow(ambient + color * illuminance, vec3(1.0/2.2));
        FragColor = vec4(color, 1.0);
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
            LEEP_CORE_ERROR("PBR vertex shader compilation failed:\n{0}\n", output_log);
        }
        //  Create and compile fragment shader and print if compilation errors
        GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag_shader, 1, &kPbrFragment, nullptr);
        glCompileShader(frag_shader);
        glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &err);
        if (!err)
        {
            glGetShaderInfoLog(frag_shader, 512, nullptr, output_log);
            LEEP_CORE_ERROR("PBR fragment shader compilation failed:\n{0}\n", output_log);
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
        Renderer& r = GM.renderer();
        LEEP_ASSERT(material.type() == MaterialType::MT_PBR, "Wrong material type");

        // Load texture
        int32_t tex_id = material.texture().id();
        LEEP_ASSERT(tex_id != -1, "Texture not created");
        LEEP_ASSERT(r.textures_[tex_id].version_ != -1, "Texture released");
        if (r.textures_[tex_id].version_ == 0)
        {
            // Render commands can be executed without submiting any DisplayList
            CreateTexture().set_texture(material.texture()).executeCommand();
        }

        GLint uniform_location = glGetUniformLocation(internal_id_, "u_albedo");
        glUniform1i(uniform_location, r.textures_[tex_id].texture_unit_);
        
        uniform_location = glGetUniformLocation(internal_id_, "u_entity_data");
        glUniform4fv(uniform_location, sizeof(PbrData) / sizeof(float) / 4, (const GLfloat*)&(material.data()));
    }
}