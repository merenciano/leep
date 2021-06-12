#version 330 core

uniform vec4 u_entity_data[6];
uniform vec4 u_scene_data[6];

layout(location=0) in vec3 a_position;
layout(location=1) in vec3 a_normal;

out vec3 vtx_position;
out vec3 vtx_normal;

void main()
{
    mat4 model = mat4(u_entity_data[0], u_entity_data[1],
                        u_entity_data[2], u_entity_data[3]);
    mat4 vp = mat4(u_scene_data[0], u_scene_data[1],
                    u_scene_data[2], u_scene_data[3]);

    vtx_position = vec3(model * vec4(a_position, 1.0));
    vtx_normal = a_normal;

    // TODO: use position
    gl_Position = vp * model * vec4(a_position, 1.0);
}
