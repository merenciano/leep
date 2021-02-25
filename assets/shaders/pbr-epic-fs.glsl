#version 330 core

#define COLOR				u_entity_data[4].xyz
#define USE_ALBEDO_MAP		u_entity_data[4].w
#define USE_PBR_MAPS		u_entity_data[5].x
#define ROUGHNESS			u_entity_data[6].x
#define METALLIC			u_entity_data[6].y
#define REFLECTANCE			u_entity_data[6].z

#define LIGHT_DIRECTION     u_scene_data[5].xyz
#define LIGHT_INTENSITY     u_scene_data[5].w
#define CAMERA_POSITION     u_scene_data[4].xyz

const float kPI = 3.14159265359;
const float kEpsilon = 0.00001;
const int kNumLights = 1;
// Constant normal incidence Fresnel factor for all dielectrics.
const vec3 kFdielectric = vec3(0.04);
const int kMaxPrefilterLod = 5;

struct AnalyticalLight {
	vec3 direction;
	vec3 radiance;
};

in Vertex
{
	mat3 tbn;
	vec3 position;
	vec2 uv;
} v_in;

out vec4 FragColor;

uniform vec4 u_entity_data[7];
uniform vec4 u_scene_data[6];
uniform sampler2D u_albedo;
uniform sampler2D u_metallic;
uniform sampler2D u_roughness;
uniform sampler2D u_normal;
uniform samplerCube u_irradiance_map;
uniform samplerCube u_prefilter_map;
uniform sampler2D u_lut;

// GGX/Towbridge-Reitz normal distribution function.
// Uses Disney's reparametrization of alpha = roughness^2.
float ndfGGX(float cosLh, float roughness)
{
	float alpha   = roughness * roughness;
	float alphaSq = alpha * alpha;

	float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
	return alphaSq / (kPI * denom * denom);
}

// Single term for separable Schlick-GGX below.
float gaSchlickG1(float cosTheta, float k)
{
	return cosTheta / (cosTheta * (1.0 - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
	float r = roughness + 1.0;
	float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
	return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}

// Shlick's approximation of the Fresnel factor.
vec3 fresnelSchlick(vec3 F0, float cosTheta)
{
	return F0 + (vec3(1.0) - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{
	// Sample input textures to get shading model params.
	vec3 albedo = texture(u_albedo, v_in.uv).rgb;
    albedo = mix(COLOR, albedo, USE_ALBEDO_MAP);
    //albedo = (1.0 - USE_ALBEDO_MAP) * COLOR + USE_ALBEDO_MAP * albedo;
	float metalness = texture(u_metallic, v_in.uv).r;
    metalness = mix(METALLIC, metalness, USE_PBR_MAPS);
	float roughness = texture(u_roughness, v_in.uv).r;
    roughness = mix(ROUGHNESS, roughness, USE_PBR_MAPS);

	// Outgoing light direction (vector from world-space fragment position to the "eye").
	vec3 Lo = normalize(CAMERA_POSITION - v_in.position);

	// Get current fragment's normal and transform to world space.
	vec3 normal = normalize(2.0 * texture(u_normal, v_in.uv).rgb - 1.0);
	normal = normalize(v_in.tbn * normal);
    normal = mix(normalize(v_in.tbn[2]), clamp(normal, -1.0, 1.0), USE_PBR_MAPS);
    
	// Angle between surface normal and outgoing light direction.
	float cosLo = max(0.0, dot(normal, Lo));
		
	// Specular reflection vector.
	vec3 Lr = 2.0 * cosLo * normal - Lo;


	// Fresnel reflectance at normal incidence (for metals use albedo color).
	vec3 F0 = mix(kFdielectric, albedo, metalness);

	// Direct lighting calculation for analytical lights.
	vec3 directLighting;
	for(int i=0; i<kNumLights; ++i)
	{
		vec3 Li = -LIGHT_DIRECTION;
		//vec3 Lradiance = vec3(1.0);
        float Lradiance = LIGHT_INTENSITY;

		// Half-vector between Li and Lo.
		vec3 Lh = normalize(Li + Lo);

		// Calculate angles between surface normal and various light vectors.
		float cosLi = max(0.0, dot(normal, Li));
		float cosLh = max(0.0, dot(normal, Lh));

		// Calculate Fresnel term for direct lighting. 
		vec3 F  = fresnelSchlick(F0, max(0.0, dot(Lh, Lo)));
		// Calculate normal distribution for specular BRDF.
		float D = ndfGGX(cosLh, roughness);
		// Calculate geometric attenuation for specular BRDF.
		float G = gaSchlickGGX(cosLi, cosLo, roughness);

		// Diffuse scattering happens due to light being refracted multiple times by a dielectric medium.
		// Metals on the other hand either reflect or absorb energy, so diffuse contribution is always zero.
		// To be energy conserving we must scale diffuse BRDF contribution based on Fresnel factor & metalness.
		vec3 kd = mix(vec3(1.0) - F, vec3(0.0), metalness);

		// Lambert diffuse BRDF.
		// We don't scale by 1/PI for lighting & material units to be more convenient.
		// See: https://seblagarde.wordpress.com/2012/01/08/pi-or-not-to-pi-in-game-lighting-equation/
		vec3 diffuseBRDF = kd * albedo;

		// Cook-Torrance specular microfacet BRDF.
		vec3 specularBRDF = (F * D * G) / max(kEpsilon, 4.0 * cosLi * cosLo);

		// Total contribution for this light.
		directLighting = (diffuseBRDF + specularBRDF) * Lradiance * cosLi;
	}

    // IBL
    // Sample diffuse irradiance at normal direction.
    vec3 irradiance = texture(u_irradiance_map, normal).rgb;

    // Calculate Fresnel term for ambient lighting.
    // Since we use pre-filtered cubemap(s) and irradiance is coming from many directions
    // use cosLo instead of angle with light's half-vector (cosLh above).
    // See: https://seblagarde.wordpress.com/2011/08/17/hello-world/
    vec3 F = fresnelSchlick(F0, cosLo);

    // Get diffuse contribution factor (as with direct lighting).
    vec3 kd = mix(vec3(1.0) - F, vec3(0.0), metalness);

    // Irradiance map contains exitant radiance assuming Lambertian BRDF, no need to scale by 1/PI here either.
    vec3 diffuseIBL = kd * albedo * irradiance;

    // Sample pre-filtered specular reflection environment at correct mipmap level.
    vec3 specularIrradiance = textureLod(u_prefilter_map, Lr, roughness * kMaxPrefilterLod).rgb;

    // Split-sum approximation factors for Cook-Torrance specular BRDF.
    vec2 specularBRDF = texture(u_lut, vec2(cosLo, roughness)).rg;

    // Total specular IBL contribution.
    vec3 specularIBL = (F0 * specularBRDF.x + specularBRDF.y) * specularIrradiance;

    // Total ambient lighting contribution.
    vec3 ambientLighting = diffuseIBL + specularIBL;
    //vec3 ambientLighting = diffuseIBL;

	// Final fragment color.
	FragColor = vec4(directLighting + ambientLighting, 1.0);
}