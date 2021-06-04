#include "render/materials/equirec-to-cube.h"
#include "core/manager.h"
#include "render/renderer.h"

#include <glad/glad.h>

static const char *kEqToCubVert = R"(
    #version 330 core
    layout (location = 0) in vec3 a_position;

    out vec3 position;

    uniform mat4 u_vp;

    void main()
    {
        position = a_position;
        gl_Position = u_vp * vec4(position, 1.0);
    }
)";

static const char *kEqToCubFrag = R"(
    #version 330 core

    uniform sampler2D u_equirectangular;

    const vec2 inv_atan = vec2(0.1591, 0.3183);

    out vec4 FragColor;
    in vec3 position;

    vec2 SampleSphericalMap(vec3 v)
    {
        vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
        uv *= inv_atan;
        uv += 0.5;
        return uv;
    }

    void main()
    {		
        vec2 uv = SampleSphericalMap(normalize(position));
        vec3 color = texture(u_equirectangular, uv).rgb;
        
        FragColor = vec4(color, 1.0);
    }
)";

namespace leep
{
    void EquirecToCube::init()
    {
        GLint err;
        GLchar output_log[512];
        GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vert_shader, 1, &kEqToCubVert, nullptr);
        glCompileShader(vert_shader);
        glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &err);
        if (!err)
        {
            glGetShaderInfoLog(vert_shader, 512, nullptr, output_log);
            LEEP_CORE_ERROR(
				"EqToCub vert shader compilation failed:\n{0}\n", output_log);
        }
        GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag_shader, 1, &kEqToCubFrag, nullptr);
        glCompileShader(frag_shader);
        glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &err);
        if (!err)
        {
            glGetShaderInfoLog(frag_shader, 512, nullptr, output_log);
            LEEP_CORE_ERROR(
                "EqToCub frag shader compilation failed:\n{0}\n", output_log);
        }
        GLuint program = glCreateProgram();
        glAttachShader(program, vert_shader);
        glAttachShader(program, frag_shader);
        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &err);
        if (!err)
        {
            glGetProgramInfoLog(program, 512, nullptr, output_log);
            LEEP_CORE_ERROR("EqToCub program error:\n{0}\n", output_log);
        }
        internal_id_ = (int32_t)program;
    }

    void EquirecToCube::useMaterialData(const Material &material) const
    {
        LEEP_CORE_ASSERT(material.type() == MaterialType::MT_EQUIREC_TO_CUBE,
            "Wrong material type");
        
        // Load texture
        Renderer &r = GM.renderer();
        int32_t tex_id = material.albedo().handle();
        LEEP_CORE_ASSERT(r.textures_[tex_id].cpu_version_ > 0, "Invalid texture");
        glUseProgram(internal_id_);
        GLint uniform_location = glGetUniformLocation(internal_id_, "u_equirectangular");
        glUniform1i(uniform_location, r.textures_[tex_id].texture_unit_);
        uniform_location = glGetUniformLocation(internal_id_, "u_vp");
        glUniformMatrix4fv(uniform_location, 1, false, (const GLfloat*)&(material.data()));
    }
}