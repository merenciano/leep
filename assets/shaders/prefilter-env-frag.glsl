#version 330 core

#define ENVIRONMENT_MAP u_entity_cube[0]
#define ROUGHNESS       u_entity_data[4].x

uniform samplerCube u_entity_cube[1];
uniform vec4 u_entity_data[5];

const float PI = 3.14159265359;
const uint kSampleCount = 1024u;

out vec4 FragColor;
in vec3 position;

float RadicalInverse_VdC(uint bits) 
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

vec2 Hammersley(uint i, uint n)
{
    return vec2(float(i)/float(n), RadicalInverse_VdC(i));
}  

vec3 ImportanceSampleGGX(vec2 xi, vec3 n, float roughness)
{
    float a = roughness*roughness;
    
    float phi = 2.0 * PI * xi.x;
    float cos_theta = sqrt((1.0 - xi.y) / (1.0 + (a*a - 1.0) * xi.y));
    float sin_theta = sqrt(1.0 - cos_theta*cos_theta);
    
    // spherical to cartesian
    vec3 h;
    h.x = cos(phi) * sin_theta;
    h.y = sin(phi) * sin_theta;
    h.z = cos_theta;
    
    // tangent-space to world-space
    vec3 up = abs(n.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent = normalize(cross(up, n));
    vec3 bitangent = cross(n, tangent);
    
    vec3 sample = tangent * h.x + bitangent * h.y + n * h.z;
    return normalize(sample);
}

void main()
{
    vec3 n = normalize(position);    
    vec3 r = n;
    vec3 v = r;

    float weight = 0.0;   
    vec3 color = vec3(0.0);     
    for(uint i = 0u; i < kSampleCount; ++i)
    {
        vec2 xi = Hammersley(i, kSampleCount);
        vec3 h  = ImportanceSampleGGX(xi, n, ROUGHNESS);
        vec3 l  = normalize(2.0 * dot(v, h) * h - v);

        float NoL = max(dot(n, l), 0.0);
        if(NoL > 0.0)
        {
            color += texture(ENVIRONMENT_MAP, l).rgb * NoL;
            weight += NoL;
        }
    }
    color = color / weight;

    FragColor = vec4(color, 1.0);
}
