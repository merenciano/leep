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
        GLuint u_loc;
        Renderer& r = GM.renderer();
        LEEP_ASSERT(material.type() == MaterialType::MT_PBR,
			"Wrong material type");

        // Load textures
        int32_t a_id = material.albedo().handle();
        if (a_id != ConstantValues::UNINITIALIZED_HANDLER)
        {
            LEEP_ASSERT(a_id != -1, "Texture not created");
            LEEP_ASSERT(r.textures_[a_id].version_ != -1, "Texture released");
            if (r.textures_[a_id].version_ == 0)
            {
                CreateTexture()
                    .set_texture(material.albedo()).executeCommand();
            }
            u_loc = glGetUniformLocation(internal_id_, "u_albedo");
            glUniform1i(u_loc, r.textures_[a_id].texture_unit_);
        }

        int32_t m_id = material.metallic().handle();
        if (m_id != ConstantValues::UNINITIALIZED_HANDLER)
        {
            LEEP_ASSERT(m_id != -1, "Texture not created");
            LEEP_ASSERT(r.textures_[m_id].version_ != -1, "Texture released");
            if (r.textures_[m_id].version_ == 0)
            {
                CreateTexture()
                    .set_texture(material.metallic()).executeCommand();
            }
            u_loc = glGetUniformLocation(internal_id_, "u_metallic");
            glUniform1i(u_loc, r.textures_[m_id].texture_unit_);
        }

        int32_t r_id = material.roughness().handle();
        if (r_id != ConstantValues::UNINITIALIZED_HANDLER)
        {
            LEEP_ASSERT(r_id != -1, "Texture not created");
            LEEP_ASSERT(r.textures_[r_id].version_!= -1, "Texture released");
            if (r.textures_[r_id].version_ == 0)
            {
                CreateTexture()
                    .set_texture(material.roughness()).executeCommand();
            }
            u_loc = glGetUniformLocation(internal_id_, "u_roughness");
            glUniform1i(u_loc, r.textures_[r_id].texture_unit_);
        }

        int32_t n_id = material.normal().handle();
        if (n_id != ConstantValues::UNINITIALIZED_HANDLER)
        {
            LEEP_ASSERT(n_id != -1, "Texture not created");
            LEEP_ASSERT(r.textures_[n_id].version_ != -1, "Texture released");
            if (r.textures_[n_id].version_ == 0)
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