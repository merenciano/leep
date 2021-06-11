#version 330 core 

layout (location = 0) in vec3 a_position;
layout (location = 2) in vec2 a_uv;

out vec2 uv;

void main()
{
    uv = a_uv;
    gl_Position = vec4(a_position.x, a_position.y, 0.0, 1.0);
}
