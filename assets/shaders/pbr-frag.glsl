#version 330 core

#define COLOR                u_entity_data[4].xyz
#define USE_ALBEDO_MAP       u_entity_data[4].w
#define USE_PBR_MAPS         u_entity_data[5].x
#define ROUGHNESS            u_entity_data[6].x
#define METALLIC             u_entity_data[6].y
#define NORMAL_MAP_INTENSITY u_entity_data[6].z

#define LIGHT_DIRECTION      u_scene_data[5].xyz
#define LIGHT_INTENSITY      u_scene_data[5].w
#define CAMERA_POSITION      u_scene_data[4].xyz

#define ALBEDO_MAP           u_entity_tex[0]
#define METALLIC_MAP         u_entity_tex[1]
#define ROUGHNESS_MAP        u_entity_tex[2]
#define NORMAL_MAP           u_entity_tex[3]

#define LUT_MAP              u_scene_tex[0]
#define IRRADIANCE_MAP       u_scene_cube[0]
#define PREFILTER_MAP        u_scene_cube[1]

const float kPI = 3.14159265359;
const float kEpsilon = 1e-5;
const vec3  kFdielectric = vec3(0.04);
const int   kMaxPrefilterLod = 5;

in Vertex
{
    mat3 tbn;
    vec3 position;
    vec2 uv;
} v_in;

out vec4 FragColor;

uniform vec4        u_entity_data[7];
uniform vec4        u_scene_data[6];

uniform sampler2D   u_entity_tex[4];
uniform sampler2D   u_scene_tex[1];
uniform samplerCube u_scene_cube[2];

// GGX NDF
float NormalDistribution(float noh, float roughness) {
    float a = noh * roughness;
    float k = roughness / ((1.0 - noh * noh) + a * a);
    return k * k * (1.0 / kPI);
}

// GGX Smith
float GeometricAttenuation(float nol, float nov, float roughness) {
    float a2 = roughness * roughness;
    float lv = nol * sqrt((nov - a2 * nov) * nov + a2);
    float ll = nov * sqrt((nol - a2 * nol) * nol + a2);
    return 0.5 / (lv + ll);
}

// Schlick's approximation
vec3 Fresnel(vec3 f0, float u)
{
    float f = pow(1.0 - u, 5.0);
    return f + f0 * (1.0 - f);
}

void main()
{
    vec3 albedo = texture(ALBEDO_MAP, v_in.uv).rgb;
    albedo = mix(COLOR, albedo, USE_ALBEDO_MAP);
    float metalness = texture(METALLIC_MAP, v_in.uv).r;
    metalness = mix(METALLIC, metalness, USE_PBR_MAPS);
    float roughness = texture(ROUGHNESS_MAP, v_in.uv).r;
    roughness = mix(ROUGHNESS, roughness, USE_PBR_MAPS);

    vec3 normal = normalize(2.0 * texture(NORMAL_MAP, v_in.uv).rgb - 1.0);
    normal = normalize(v_in.tbn * normal);
    normal = mix(normalize(v_in.tbn[2]), clamp(normal, -1.0, 1.0), NORMAL_MAP_INTENSITY);


    // Fresnel at normal incidence
    vec3 f0 = mix(kFdielectric, albedo, metalness);

    vec3 view = normalize(CAMERA_POSITION - v_in.position);
    vec3 light = -LIGHT_DIRECTION;
    // Half vec between light and view 
    vec3 hlv = normalize(light + view);

    float nov = abs(dot(normal, view)) + kEpsilon;
    float nol = clamp(dot(normal, light), 0.0, 1.0);
    float noh = clamp(dot(normal, hlv), 0.0, 1.0);
    float loh = clamp(dot(light, hlv), 0.0, 1.0);

    roughness = roughness * roughness;
    vec3  f = Fresnel(f0, loh);
    float d = NormalDistribution(noh, roughness);
    float g = GeometricAttenuation(nol, nov, roughness);

    // Diffuse
    vec3 kd = mix(vec3(1.0) - f, vec3(0.0), metalness);
    vec3 diffuse_brdf = kd * albedo;

    // Specular
    vec3 specular_brdf = ((d * g) * f) / max(kEpsilon, 4.0 * nol * nov);
    vec3 direct_lighting = (diffuse_brdf + specular_brdf) * LIGHT_INTENSITY * nol;

    // IBL
    vec3 irradiance = texture(IRRADIANCE_MAP, normal).rgb;
    vec3 f_ibl = Fresnel(f0, nov);
    vec3 kd_ibl = mix(vec3(1.0) - f_ibl, vec3(0.0), metalness);
    vec3 diffuse_ibl = kd_ibl * albedo * irradiance;

    vec3 r = reflect(-view, normal);
    vec3 specular_irradiance = textureLod(PREFILTER_MAP, r, roughness * kMaxPrefilterLod).rgb;
    vec2 specular_brdf_ibl = texture(LUT_MAP, vec2(nov, roughness)).rg;
    vec3 specular_ibl = (f0 * specular_brdf_ibl.x + specular_brdf_ibl.y) * specular_irradiance;
    vec3 ambient_lighting = diffuse_ibl + specular_ibl;

    FragColor = vec4(direct_lighting + ambient_lighting, 1.0);
}
