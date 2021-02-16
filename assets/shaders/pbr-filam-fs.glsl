#version 330 core 

#define CAMERA_POSITION     u_scene_data[4].xyz
#define LIGHT_DIRECTION     u_scene_data[5].xyz
#define LIGHT_INTENSITY     u_scene_data[5].w

#define COLOR				u_entity_data[4].xyz
#define USE_ALBEDO_MAP		u_entity_data[4].w
#define USE_PBR_MAPS		u_entity_data[5].x

#define ROUGHNESS			u_entity_data[6].x
#define METALLIC			u_entity_data[6].y
#define REFLECTANCE			u_entity_data[6].z

#define PI 3.14159265359

uniform vec4 u_entity_data[7];
uniform vec4 u_scene_data[6];
uniform sampler2D u_albedo;
uniform sampler2D u_metallic;
uniform sampler2D u_roughness;
uniform sampler2D u_normal;

in Vertex
{
	mat3 tbn;
	vec3 position;
	vec2 uv;
} v_in;

out vec4 FragColor;

struct Material
{
    vec3  albedo;
    float roughness;
    float metallic;
    float reflectance;
};

// Normal distribution function (specular D)
float D_GGX(float NoH, float roughness) {
    float a = NoH * roughness;
    float k = roughness / ((1.0 - NoH * NoH) + a * a);
    return k * k * (1.0 / PI);
}

float V_SmithGGXCorrelated(float NoV, float NoL, float roughness) {
    float a2 = roughness * roughness;
    float lambdaV = NoL * sqrt((NoV - a2 * NoV) * NoV + a2);
    float lambdaL = NoV * sqrt((NoL - a2 * NoL) * NoL + a2);
    return 0.5 / (lambdaV + lambdaL);
}

// Visibility (specular V)
float V_SmithGGXCorrelatedFast(float NoV, float NoL, float roughness) {
    float a = roughness;
    float GGXV = NoL * (NoV * (1.0 - a) + a);
    float GGXL = NoV * (NoL * (1.0 - a) + a);
    return 0.5 / (GGXV + GGXL); // TODO: Test with max(that, 0.01)
}

// Fresnel
vec3 F_Schlick(float u, vec3 f0) {
    float f = pow(1.0 - u, 5.0);
    return f + f0 * (1.0 - f);
}

// Diffuse BRDF
float Fd_Lambert() {
    return 1.0 / PI;
}

vec3 BRDF(Material mat, vec3 l, vec3 v, vec3 n) {
    vec3 h = normalize(v + l);

    vec3 f0 = 0.16 * mat.reflectance * mat.reflectance * (1.0 - mat.metallic) + mat.albedo * mat.metallic;

    float NoV = abs(dot(n, v)) + 1e-5;
    float NoL = clamp(dot(n, l), 0.0, 1.0);
    float NoH = clamp(dot(n, h), 0.0, 1.0);
    float LoH = clamp(dot(l, h), 0.0, 1.0);

    // perceptually linear roughness to roughness
    float roughness = mat.roughness * mat.roughness;

    float D = D_GGX(NoH, roughness);
    vec3  F = F_Schlick(LoH, f0);
    float V = V_SmithGGXCorrelated(NoV, NoL, roughness);
    // specular BRDF
    vec3 Fr = (D * V) * F;

    // diffuse BRDF
    vec3 Fd = mat.albedo * Fd_Lambert();

    // apply lighting...
    return Fd + Fr;
}

void main() {
    float roughness = texture(u_roughness, vec2(v_in.uv.x, v_in.uv.y)).r;
    vec3 normal = texture(u_normal, vec2(v_in.uv.x, v_in.uv.y)).rgb;
    normal = normalize(v_in.tbn * (normal * 2.0 - 1.0));
    float metallic = texture(u_metallic, vec2(v_in.uv.x, v_in.uv.y)).r;
    Material material;
    material.roughness = max(0.05, mix(ROUGHNESS, roughness, USE_PBR_MAPS));
    material.metallic = mix(METALLIC, metallic, USE_PBR_MAPS);
    material.reflectance = REFLECTANCE;

    vec3 albedo = texture(u_albedo, vec2(v_in.uv.x, v_in.uv.y)).rgb;
    material.albedo = (1.0 - USE_ALBEDO_MAP) * COLOR + USE_ALBEDO_MAP * albedo;
    material.albedo *= (1.0 - material.metallic);

    vec3 l = normalize(-LIGHT_DIRECTION);
    float NoL = clamp(dot(normal, l), 0.0, 1.0);
    float illuminance = LIGHT_INTENSITY * NoL;
    vec3 color = BRDF(material, l, normalize(CAMERA_POSITION - vec3(v_in.position)), normal);
    vec3 ambient = material.albedo * 0.03;

    //FragColor = vec4(normal, 1.0);
    FragColor = vec4(ambient + color * illuminance, 1.0);
}