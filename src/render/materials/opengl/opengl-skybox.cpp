#include "render/materials/skybox.h"
#include "core/common-defs.h"
#include "core/manager.h"
#include "core/logger.h"
#include "render/renderer.h"
#include "render/commands/create-cubemap.h"

#include <glad/glad.h>

static const char* kSkyboxVertex = R"(
    #version 330

    uniform vec4 u_scene_data[4];

    layout (location = 0) in vec3 a_position;
    layout (location = 1) in vec3 a_normal;
    layout (location = 2) in vec2 a_uv;

    out vec3 position;

    void main() {
        mat4 vp = mat4(u_scene_data[0], u_scene_data[1], u_scene_data[2], u_scene_data[3]);
        position = a_position;
        gl_Position = vec4(vp * vec4(position, 1.0)).xyww;
    }
)";

static const char* kSkyboxFragment = R"(
    #version 330

    uniform samplerCube u_cubemap;

    out vec4 FragColor;
    in vec3 position;

    void main() {
        FragColor = texture(u_cubemap, position);
    }
)";

namespace leep
{
    void Skybox::init()
    {
        GLint err;
        GLchar output_log[512];
        //  Create and compile vertex shader and print if compilation errors
        GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vert_shader, 1, &kSkyboxVertex, nullptr);
        glCompileShader(vert_shader);
        glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &err);
        if (!err)
        {
            glGetShaderInfoLog(vert_shader, 512, nullptr, output_log);
            LEEP_CORE_ERROR("Skybox vertex shader compilation failed:\n{0}\n", output_log);
        }
        //  Create and compile fragment shader and print if compilation errors
        GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag_shader, 1, &kSkyboxFragment, nullptr);
        glCompileShader(frag_shader);
        glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &err);
        if (!err)
        {
            glGetShaderInfoLog(frag_shader, 512, nullptr, output_log);
            LEEP_CORE_ERROR("Skybox fragment shader compilation failed:\n{0}\n", output_log);
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
            LEEP_CORE_ERROR("Skybox program error:\n{0}\n", output_log);
        }

        internal_id_ = (uint32_t)program;
    }

    void Skybox::useMaterialData(const Material& material) const
    {
        Renderer& r = GM.renderer();
        LEEP_ASSERT(material.type() == MaterialType::MT_SKYBOX, "Wrong material type");

        // Load texture
        int32_t tex_id = material.albedo().handle();
        LEEP_ASSERT(tex_id != -1, "Texture not created");
        LEEP_ASSERT(r.textures_[tex_id].cpu_version_ != -1, "Texture released");
        if (r.textures_[tex_id].cpu_version_ == 0)
        {
            // Render commands can be executed without submiting any DisplayList,
            // you just have to make sure that the command will be executed from the main thread
            CreateCubemap().set_texture(material.albedo()).executeCommand();
        }

        GLint uniform_location = glGetUniformLocation(internal_id_, "u_cubemap");
        glUniform1i(uniform_location, r.textures_[tex_id].texture_unit_);
    }
}