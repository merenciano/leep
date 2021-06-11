#version 330 core

#define TILING_X u_entity_data[5].y
#define TILING_Y u_entity_data[5].z

uniform vec4 u_entity_data[7];
uniform vec4 u_scene_data[6];

layout(location=0) in vec3 a_position;
layout(location=1) in vec3 a_normal;
layout(location=2) in vec3 a_tangent;
layout(location=3) in vec3 a_bitangent;
layout(location=4) in vec2 a_uv;

out Vertex
{
    mat3 tbn;
    vec3 position;
    vec2 uv;
} v_out;

void main()
{
    mat4 model = mat4(u_entity_data[0], u_entity_data[1],
                        u_entity_data[2], u_entity_data[3]);
    mat4 vp = mat4(u_scene_data[0], u_scene_data[1],
                    u_scene_data[2], u_scene_data[3]);

    v_out.position = vec3(model * vec4(a_position, 1.0));
    v_out.uv = vec2(a_uv.x * TILING_X, a_uv.y * TILING_Y);

    // Pass tangent space basis vectors (for normal mapping).
    v_out.tbn = mat3(model) * mat3(normalize(a_tangent), normalize(a_bitangent), normalize(a_normal));

    gl_Position = vp * model * vec4(a_position, 1.0);
}
