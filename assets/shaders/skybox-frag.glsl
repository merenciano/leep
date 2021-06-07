#version 330

#define SKYBOX_CUBEMAP u_entity_cube[0]

uniform samplerCube u_entity_cube[1];

out vec4 FragColor;
in vec3 position;

void main() 
{
    FragColor = texture(SKYBOX_CUBEMAP, position);
}
