#version 330 core 

in vec2 uv;
out vec2 FragColor;

const float kTwoPI = 2.0 * 3.14159265359;
const uint  kSampleCount = 1024u;
const float kInvSampleCount = 1.0 / float(kSampleCount);
const float kEpsilon = 0.001; // This program needs larger eps.

float RadicalInverse_VdC(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

// Sample i-th point from Hammersley point set of NumSamples points total.
vec2 SampleHammersley(uint i)
{
    return vec2(i * kInvSampleCount, RadicalInverse_VdC(i));
}

vec3 SampleGGX(float u1, float u2, float roughness)
{
    float alpha = roughness * roughness;

    float cos_theta = sqrt((1.0 - u2) / (1.0 + (alpha*alpha - 1.0) * u2));
    float sin_theta = sqrt(1.0 - cos_theta*cos_theta);
    float phi = kTwoPI * u1;

    // Convert to Cartesian upon return.
    return vec3(sin_theta * cos(phi), sin_theta * sin(phi), cos_theta);
}

// Single term for separable Schlick-GGX below.
float GASchlickG1(float cos_theta, float k)
{
    return cos_theta / (cos_theta * (1.0 - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method (IBL version).
float GASchlickGGX_IBL(float cos_li, float cos_lo, float roughness)
{
    float r = roughness;
    float k = (r * r) / 2.0;
    return GASchlickG1(cos_li, k) * GASchlickG1(cos_lo, k);
}

void main() {
    float roughness = uv.y;
    float cos_lo = max(uv.x, kEpsilon);

    // viewing direction
    vec3 lo = vec3(sqrt(1.0 - cos_lo*cos_lo), 0.0, cos_lo);

    float dfg1 = 0;
    float dfg2 = 0;

    for(uint i=0u; i < kSampleCount; ++i) {
        vec2 u = SampleHammersley(i);
        // half-vector
        vec3 lh = SampleGGX(u.x, u.y, roughness);

        // Incident direction
        vec3 li = 2.0 * dot(lo, lh) * lh - lo;

        float cos_li = li.z;
        float cos_lh = lh.z;
        float cos_lo_lh = max(dot(lo, lh), 0.0);

        if(cos_li > 0.0)
        {
            float g  = GASchlickGGX_IBL(cos_li, cos_lo, roughness);
            float gv = g * cos_lo_lh / (cos_lh * cos_lo);
            float fc = pow(1.0 - cos_lo_lh, 5);

            dfg1 += (1 - fc) * gv;
            dfg2 += fc * gv;
        }
    }

    FragColor = vec2(dfg1 * kInvSampleCount, dfg2 * kInvSampleCount);
}
