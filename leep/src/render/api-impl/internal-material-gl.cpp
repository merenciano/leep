#include "core/string.h"
#include "render/renderer.h"
#include "render/commands/create-texture.h"

#include <glad/glad.h>

namespace leep {

void InternalMaterial::init(const char *name)
{
    char *vert, *frag;
    String path("../assets/shaders/");
    path.append(String(name));
#ifdef LEEP_OPENGL
    LoadFile((path + "-vert.glsl").c_str(), &vert);
    LoadFile((path + "-frag.glsl").c_str(), &frag);
#else
    LoadFile((path + "-vert-es.glsl").c_str(), &vert);
    LoadFile((path + "-frag-es.glsl").c_str(), &frag);
#endif

    GLint err;
    GLchar output_log[512];
    //  Create and compile vertex shader and print if compilation errors
    GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vert, nullptr);
    glCompileShader(vert_shader);
    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &err);
    if (!err)
    {
        glGetShaderInfoLog(vert_shader, 512, nullptr, output_log);
        LEEP_CORE_ERROR("{0} vertex shader compilation failed:\n{1}\n", name, output_log);
    }
    //  Create and compile fragment shader and print if compilation errors
    GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag, nullptr);
    glCompileShader(frag_shader);
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &err);
    if (!err)
    {
        glGetShaderInfoLog(frag_shader, 512, nullptr, output_log);
        LEEP_CORE_ERROR("{0} fragment shader compilation failed:\n{1}\n", name, output_log);
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
        LEEP_CORE_ERROR("{0} program error:\n{1}\n", name, output_log);
    }

    internal_id_ = (uint32_t)program;
    GM.memory().generalFree(vert);
    GM.memory().generalFree(frag);
}

void InternalMaterial::useMaterialData(const Material &m) const
{
    GLint u_loc;
    Renderer& r = GM.renderer();

    // Load textures
    LEEP_CORE_ASSERT(m.tcount() < 16,
        "So many textures, increase array size");
    int32_t tex_units[16];
    for (int32_t i = 0; i < m.tcount(); ++i)
    {
        int32_t handle = m.tex()[i].handle();
        if (handle != CommonDefs::UNINIT_HANDLE)
        {
            LEEP_ASSERT(handle != -1, "Texture not created");
            LEEP_ASSERT(r.textures_[handle].cpu_version_ != -1, "Texture released");
            if (r.textures_[handle].gpu_version_ == 0)
            {
                CreateTexture()
                    .set_texture(m.tex()[i]).executeCommand();
            }
        }
        tex_units[i] = r.textures_[handle].texture_unit_;
    }

    // TODO: Change enum values and if type < MT_ENGINE_MAX
    if (m.type() == MaterialType::MT_FULL_SCREEN_IMAGE ||
        m.type() == MaterialType::MT_SKYBOX ||
        m.type() == MaterialType::MT_EQUIREC_TO_CUBE ||
        m.type() == MaterialType::MT_PREFILTER_ENV ||
        m.type() == MaterialType::MT_LUT_GEN)
    {
        glUseProgram(internal_id_);
    }

    u_loc = glGetUniformLocation(internal_id_, "u_entity_data");
    if (u_loc >= 0)
    {
        glUniform4fv(u_loc, m.dcount() / 4, m.data());
    }

    u_loc = glGetUniformLocation(internal_id_, "u_entity_tex");
    if (u_loc >= 0)
    {
        glUniform1iv(u_loc, m.cube_start(), tex_units);
    }

    u_loc = glGetUniformLocation(internal_id_, "u_entity_cube");
    if (u_loc >= 0)
    {
        glUniform1iv(u_loc, m.tcount() - m.cube_start(),
                     tex_units + m.cube_start());
    }

    if (m.type() == MaterialType::MT_FULL_SCREEN_IMAGE)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

} // end namespace leep