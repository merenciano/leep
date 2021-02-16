#include "render/materials/pbr.h"
#include "core/common-defs.h"
#include "core/manager.h"
#include "core/logger.h"
#include "render/renderer.h"
#include "render/commands/create-texture.h"

#include <fstream>
#include <glad/glad.h>

static const char* kPbrVertex = R"(
)";

static const char* kPbrFragment = R"(
)";

namespace leep
{
    void Pbr::init()
    {
        std::ifstream vifs("../assets/shaders/pbr-epic-vs.glsl");
        std::string vert((std::istreambuf_iterator<char>(vifs)),
                         (std::istreambuf_iterator<char>()));
        std::ifstream fifs("../assets/shaders/pbr-epic-fs.glsl");
        std::string frag((std::istreambuf_iterator<char>(fifs)),
                         (std::istreambuf_iterator<char>()));
        const GLchar *vert_str = vert.c_str();
        const GLchar *frag_str = frag.c_str();
        GLint err;
        GLchar output_log[512];
        //  Create and compile vertex shader and print if compilation errors
        GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vert_shader, 1, &vert_str, nullptr);
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
        glShaderSource(frag_shader, 1, &frag_str, nullptr);
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
        Renderer& r = GM.renderer();
        LEEP_ASSERT(material.type() == MaterialType::MT_PBR,
			"Wrong material type");

        // Load texture
        int32_t albedo_id = material.albedo().handle();
        int32_t metallic_id = material.metallic().handle();
        int32_t roughness_id = material.roughness().handle();
        int32_t normal_id = material.normal().handle();
        LEEP_ASSERT(albedo_id != -1, "Texture not created");
        LEEP_ASSERT(metallic_id != -1, "Texture not created");
        LEEP_ASSERT(roughness_id != -1, "Texture not created");
        LEEP_ASSERT(normal_id != -1, "Texture not created");
        LEEP_ASSERT(r.textures_[albedo_id].version_ != -1, "Texture released");
        LEEP_ASSERT(r.textures_[metallic_id].version_ != -1,"Texture released");
        LEEP_ASSERT(r.textures_[roughness_id].version_!= -1,"Texture released");
        LEEP_ASSERT(r.textures_[normal_id].version_ != -1, "Texture released");
        if (r.textures_[albedo_id].version_ == 0)
        {
			CreateTexture()
				.set_texture(material.albedo())
				.executeCommand();
        }

        if (r.textures_[metallic_id].version_ == 0)
        {
			CreateTexture()
				.set_texture(material.metallic())
				.executeCommand();
        }

        if (r.textures_[roughness_id].version_ == 0)
        {
			CreateTexture()
				.set_texture(material.roughness())
				.executeCommand();
        }

        if (r.textures_[normal_id].version_ == 0)
        {
			CreateTexture()
				.set_texture(material.normal())
				.executeCommand();
        }

        GLint uniform_location = glGetUniformLocation(internal_id_, "u_albedo");
        glUniform1i(uniform_location, r.textures_[albedo_id].texture_unit_);
        uniform_location = glGetUniformLocation(internal_id_, "u_metallic");
        glUniform1i(uniform_location, r.textures_[metallic_id].texture_unit_);
        uniform_location = glGetUniformLocation(internal_id_, "u_roughness");
        glUniform1i(uniform_location, r.textures_[roughness_id].texture_unit_);
        uniform_location = glGetUniformLocation(internal_id_, "u_normal");
        glUniform1i(uniform_location, r.textures_[normal_id].texture_unit_);
        
        uniform_location = glGetUniformLocation(internal_id_, "u_entity_data");
        glUniform4fv(uniform_location, sizeof(PbrData) / sizeof(float) / 4,
					 (const GLfloat*)&(material.data()));
    }
}