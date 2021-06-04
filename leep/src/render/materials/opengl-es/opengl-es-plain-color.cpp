#include "render/materials/plain-color.h"

#include "core/logger.h"
#include "core/common-defs.h"

#include "glad/glad.h"

static const char* kPlainColorVertex = R"(
    #version 300 es 
    precision mediump float;

    uniform vec4 u_entity_data[5];
    uniform vec4 u_scene_data[4];

    layout (location = 0) in vec3 a_position;
    layout (location = 1) in vec3 a_normal;
    layout (location = 2) in vec2 a_uv;

    out vec4 color;

    void main() {
        mat4 world = mat4(u_entity_data[0], u_entity_data[1], u_entity_data[2], u_entity_data[3]);
        mat4 vp = mat4(u_scene_data[0], u_scene_data[1], u_scene_data[2], u_scene_data[3]);
        vec4 world_position = world * vec4(a_position, 1.0);
        color = u_entity_data[4];
        gl_Position = vp * world_position;
    }
)";

static const char* kPlainColorFragment = R"(
    #version 300 es 
    precision mediump float;

    in vec4 color;
    out vec4 FragColor;

    void main() {
        FragColor = color; 
    }
)";

namespace leep
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
            LEEP_CORE_ERROR("PlainColor vertex shader compilation failed:\n{0}\n", output_log);
        }
        //  Create and compile fragment shader and print if compilation errors
        GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag_shader, 1, &kPlainColorFragment, nullptr);
        glCompileShader(frag_shader);
        glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &err);
        if (!err)
        {
            glGetShaderInfoLog(frag_shader, 512, nullptr, output_log);
            LEEP_CORE_ERROR("PlainColor fragment shader compilation failed:\n{0}\n", output_log);
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
            LEEP_CORE_ERROR("PlainColor program error:\n{0}\n", output_log);
        }

        internal_id_ = (uint32_t)program;
    }

    void PlainColor::useMaterialData(const Material &material) const
    {
        LEEP_ASSERT(material.type() == MaterialType::MT_PLAIN_COLOR,
            "Wrong material type");
        
        GLint uniform_location = glGetUniformLocation(internal_id_, "u_entity_data");
        glUniform4fv(uniform_location, 5, (const GLfloat*)&(material.data()));
    }
}