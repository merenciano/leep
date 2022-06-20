#include "helper-functions.h"

#include "core/manager.h"
#include "core/scene.h"
#include "ecs/entity.h"
#include "ecs/components/drawable.h"
#include "tools/resource-map.h"

void leep::CreateRenderable(String entity_name,
                            String geometry_name,
                            String texture_name)
{
    Entity e = Entity::CreateEntity(entity_name, EntityType::RENDERABLE);
    // If it fails
    if (!e.isValid())
    {
        THE_SLOG_WARNING("CreateRenderable: The entity couldn't be created");
        return;
    }
    Drawable &d = e.getComponent<Drawable>();
    THE_PbrData pbr;
    pbr.color = svec3(1.0f, 1.0f, 1.0f);
    pbr.use_albedo_map = 1.0f;
    pbr.use_pbr_maps = 0.0f;
    pbr.tiling_x = 1.0f;
    pbr.tiling_y = 1.0f;
    pbr.metallic = 0.5f;
    pbr.roughness = 0.5f;
    pbr.normal_map_intensity = 0.0f;
    //d.mesh = GM.resource_map().getGeometry(geometry_name);
    d.mesh = GM.resource_map()->meshes.at(geometry_name);
    d.mat.type = THE_MT_PBR;
    THE_MaterialSetData(&d.mat, (float*)&pbr, sizeof(THE_PbrData) / 4);
//    d.mat.set_data((float*)&pbr, sizeof(PbrData) / 4);
    THE_Texture t[4];
    for (int32_t i = 0; i < 4; ++i)
    {
        //t[i] = GM.resource_map().getTexture(texture_name);
	t[i] = GM.resource_map()->textures.at(texture_name);
    }
    THE_MaterialSetTexture(&d.mat, t, 4);
    //d.mat.set_tex(t, 4);
}

void leep::RemoveEntity(String entity_name)
{
    Entity::RemoveEntity(entity_name);
}

void leep::SetParent(String entity_name, String parent_name)
{
    LTransform &eltr = Entity::GetEntity(entity_name).getComponent<LTransform>();
    LTransform &pltr = Entity::GetEntity(parent_name).getComponent<LTransform>();
    GTransform &egtr = Entity::GetEntity(entity_name).getComponent<GTransform>();
    GTransform &pgtr = Entity::GetEntity(parent_name).getComponent<GTransform>();
    GM.scene().scene_graph_.createNode(&eltr, &egtr);
    GM.scene().scene_graph_.createNode(&pltr, &pgtr);
    GM.scene().scene_graph_.setParent(&eltr, &pltr);
}

void leep::DetachFromParent(String entity_name)
{
    LTransform &eltr = Entity::GetEntity(entity_name).getComponent<LTransform>();
    GM.scene().scene_graph_.detachFromParent(&eltr);
}

void leep::SetLocation(String entity_name, float x, float y, float z)
{
    LTransform &eltr = Entity::GetEntity(entity_name).getComponent<LTransform>();
    eltr.setLocation(glm::vec3(x, y, z));
}

void leep::SetSunDirection(float x, float y, float z)
{
	sun_dir_intensity.x = x;
	sun_dir_intensity.y = y;
	sun_dir_intensity.z = z;
}

void leep::SetSunIntensity(float intensity)
{
	sun_dir_intensity.w = intensity;
}
