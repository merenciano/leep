#include "helper-functions.h"
#include "tools/resource-map.h"
#include "core/manager.h"
#include "ecs/entity.h"

void THE_CreateEntity(const char *mesh, const char *tex)
{
    THE_Entity *e = THE_EntityCreate();
    THE_PbrData pbr;
    pbr.color = svec3(1.0f, 1.0f, 1.0f);
    pbr.use_albedo_map = 1.0f;
    pbr.use_pbr_maps = 0.0f;
    pbr.tiling_x = 1.0f;
    pbr.tiling_y = 1.0f;
    pbr.metallic = 0.5f;
    pbr.roughness = 0.5f;
    pbr.normal_map_intensity = 0.0f;
    e->mesh = THE_ResourceMapGetMesh(&resource_map, mesh);
    e->mat.type = THE_MT_PBR;
    THE_MaterialSetData(&e->mat, (float*)&pbr, sizeof(THE_PbrData) / 4);
    THE_Texture t[4];
    for (s32 i = 0; i < 4; ++i) {
        t[i] = THE_ResourceMapGetTexture(&resource_map, tex);
    }
    THE_MaterialSetTexture(&e->mat, t, 4, -1);
}

void THE_SetLocation(s32 entity_idx, float x, float y, float z)
{
    float newpos[3] = {x, y, z};
    mat4_translation((float*)&(THE_GetEntities()[entity_idx].transform),
        (float*)&(THE_GetEntities()[entity_idx].transform), newpos);
}

void THE_SetSunDirection(float x, float y, float z)
{
	sun_dir_intensity.x = x;
	sun_dir_intensity.y = y;
	sun_dir_intensity.z = z;
}

void THE_SetSunIntensity(float intensity)
{
	sun_dir_intensity.w = intensity;
}
