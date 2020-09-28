#include "render/materials/plain-color.h"

#include "core/logger.h"
#include "core/common-defs.h"

#include "glad/glad.h"

static const char* kPlainColorVertex = R"(
    #version 330 core 

    layout (location = 0) in vec3 a_position;
    layout (location = 1) in vec3 a_normal;
    layout (location = 2) in vec2 a_uv;

    void main() {
        gl_Position = vec4(position, 1.0);
    }
)";

static const char* kPlainColorFragment = R"(
    #version 330 core 

    uniform vec4 u_color;

    out vec4 FragColor;

    void main() {
        FragColor = u_color; 
    }
)";

namespace motoret
{
    void PlainColor::init()
    {
        GLint err;
        GLchar output_log[512];
        //  Create and compile vertex shader and print if compilation errors
        GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vert_shader, 1, &kPlainColorVertex, nullptr);
        glCompileShader(vert_shader);
        glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &err);
        if (!err)
        {
            glGetShaderInfoLog(vert_shader, 512, nullptr, output_log);
            MOTORET_CORE_ERROR("PlainColor vertex shader compilation failed:\n%s\n", output_log);
        }
        //  Create and compile fragment shader and print if compilation errors
        GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag_shader, 1, &kPlainColorFragment, nullptr);
        glCompileShader(frag_shader);
        glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &err);
        if (!err)
        {
            glGetShaderInfoLog(frag_shader, 512, nullptr, output_log);
            MOTORET_CORE_ERROR("PlainColor fragment shader compilation failed:\n%s\n", output_log);
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
            MOTORET_CORE_ERROR("PlainColor program error:\n%s\n", output_log);
        }

        internal_id_ = (uint32_t)program;
    }

    void PlainColor::useMaterialData(Material material) const
    {
        MOTORET_ASSERT(material.type() == MaterialType::PLAIN_COLOR,
            "Wrong material type");
        
        GLint uniform_location = glGetUniformLocation(internal_id_, "u_color");
        glUniform4f(uniform_location,
            material.data().plain_color.r,
            material.data().plain_color.g,
            material.data().plain_color.b,
            material.data().plain_color.a);
    }
}