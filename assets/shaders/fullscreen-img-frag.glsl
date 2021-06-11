#version 330 core 

uniform sampler2D u_entity_tex[1];

in vec2 uv;

out vec4 FragColor;

void main()
{
    vec3 color = texture(u_entity_tex[0], uv).rgb;

    // Tone mapping
    color = color / (color + vec3(1.0));

    // gamma correction
    color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color,  1.0);
}
