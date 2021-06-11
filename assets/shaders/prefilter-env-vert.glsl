#version 330 core

layout (location = 0) in vec3 a_position;

uniform vec4 u_entity_data[5];

out vec3 position;

void main()
{
    mat4 vp = mat4(u_entity_data[0], u_entity_data[1], u_entity_data[2], u_entity_data[3]);
    position = a_position;
    gl_Position = vp * vec4(position, 1.0);
}