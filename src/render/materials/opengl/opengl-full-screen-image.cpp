#include "render/materials/full-screen-image.h"

#include "core/logger.h"
#include "core/common-defs.h"
#include "core/manager.h"
#include "render/renderer.h"
#include "render/commands/create-texture.h"

#include <glad/glad.h>

static const char* kFSImageVertex = R"(
    #version 330 core 

    layout (location = 0) in vec3 a_position;
    layout (location = 2) in vec2 a_uv;

    out vec2 uv;

    void main() {
        uv = a_uv;
        gl_Position = vec4(a_position.x, a_position.y, 0.0, 1.0);
    }
)";

static const char* kFSImageFragment = R"(
    #version 330 core 

    uniform sampler2D u_texture;
    in vec2 uv;

    out vec4 FragColor;

    void main() {
        vec3 color = texture(u_texture, uv).rgb;

        // Tone mapping
        color = color / (color + vec3(1.0));

        // gamma correction
        color = pow(color, vec3(1.0/2.2));

        FragColor = vec4(color,  1.0);
    }
)";

namespace leep
{
    void FullScreenImage::init()
    {
        GLint err;
        GLchar output_log[512];
        //  Create and compile vertex shader and print if compilation errors
        GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vert_shader, 1, &kFSImageVertex, nullptr);
        glCompileShader(vert_shader);
        glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &err);
        if (!err)
        {
            glGetShaderInfoLog(vert_shader, 512, nullptr, output_log);
            LEEP_CORE_ERROR("Fullscreen image vertex shader compilation failed:\n{0}\n", output_log);
        }
        //  Create and compile fragment shader and print if compilation errors
        GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag_shader, 1, &kFSImageFragment, nullptr);
        glCompileShader(frag_shader);
        glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &err);
        if (!err)
        {
            glGetShaderInfoLog(frag_shader, 512, nullptr, output_log);
            LEEP_CORE_ERROR("Fullscreen image fragment shader compilation failed:\n{0}\n", output_log);
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
            LEEP_CORE_ERROR("Fullscreen image program error:\n{0}\n", output_log);
        }

        internal_id_ = (uint32_t)program;
    }

    void FullScreenImage::useMaterialData(const Material &material) const
    {
        LEEP_CORE_ASSERT(material.type() == MaterialType::MT_FULL_SCREEN_IMAGE,
            "Wrong material type");
        
        // Load texture
        Renderer &r = GM.renderer();
        int32_t tex_id = material.albedo().handle();
        LEEP_CORE_ASSERT(r.textures_[tex_id].cpu_version_ > 0, "Invalid texture");
        glUseProgram(internal_id_);
        GLint uniform_location = glGetUniformLocation(internal_id_, "u_texture");
        glUniform1i(uniform_location, r.textures_[tex_id].texture_unit_);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}