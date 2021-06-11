#version 330 core

#define EQUIREC_ENV_MAP u_entity_tex[0]

uniform sampler2D u_entity_tex[1];

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
    vec3 color = texture(EQUIREC_ENV_MAP, uv).rgb;
    
    FragColor = vec4(color, 1.0);
}
