#include "render/materials/pbr.h"
#include "core/common-defs.h"
#include "core/manager.h"
#include "core/logger.h"
#include "render/commands/create-texture.h"

#include "glad/glad.h"

static const char* kPbrVertex = R"(
    #version 300 es
    precision mediump float; 

    uniform vec4 u_entity_data[5];
    uniform vec4 u_scene_data[4];

    layout (location = 0) in vec3 a_position;
    layout (location = 1) in vec3 a_normal;
    layout (location = 2) in vec2 a_uv;

    out vec2 uv;

    void main() {
        mat4 world = mat4(u_entity_data[0], u_entity_data[1], u_entity_data[2], u_entity_data[3]);
        mat4 vp = mat4(u_scene_data[0], u_scene_data[1], u_scene_data[2], u_scene_data[3]);
        vec4 world_position = world * vec4(a_position, 1.0);
        uv = a_uv;
        gl_Position = vp * world_position;
    }
)";

static const char* kPbrFragment = R"(
    #version 300 es
    precision mediump float;

    uniform vec4 u_entity_data[5];
    uniform sampler2D u_texture;

    in vec2 uv;
    out vec4 FragColor;

    void main() {
        vec3 color = texture(u_texture, vec2(uv.x * u_entity_data[4].x, uv.y * u_entity_data[4].y)).rgb;
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
        int32_t tex_id = material.albedo().handle();
        LEEP_ASSERT(tex_id != -1, "Texture not created");
        LEEP_ASSERT(r.textures_[tex_id].cpu_version_ != -1, "Texture released");
        if (r.textures_[tex_id].cpu_version_ == 0)
        {
            // Render commands can be executed without submiting any DisplayList
            CreateTexture().set_texture(material.albedo()).executeCommand();
        }

        GLint uniform_location = glGetUniformLocation(internal_id_, "u_texture");
        glUniform1i(uniform_location, r.textures_[tex_id].texture_unit_);
        
        uniform_location = glGetUniformLocation(internal_id_, "u_entity_data");
        glUniform4fv(uniform_location, 5, (const GLfloat*)&(material.data()));
    }
}