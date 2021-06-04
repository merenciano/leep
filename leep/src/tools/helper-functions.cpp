#include "helper-functions.h"

#include "core/manager.h"
#include "core/scene.h"
#include "ecs/entity.h"
#include "ecs/components/drawable.h"
#include "render/renderer.h"
#include "tools/resource-map.h"

void leep::CreateRenderable(String entity_name,
                            String geometry_name,
                            String texture_name)
{
    Entity e = Entity::CreateEntity(entity_name, EntityType::RENDERABLE);
    // If it fails
    if (!e.isValid())
    {
        LEEP_CORE_WARNING("CreateRenderable: The entity couldn't be created");
        return;
    }
    Drawable &d = e.getComponent<Drawable>();
    PbrData pbr;
    pbr.color_ = glm::vec3(1.0f, 1.0f, 1.0f);
    pbr.use_albedo_map_ = 1.0f;
    pbr.use_pbr_maps_ = 0.0f;
    pbr.tiling_x_ = 1.0f;
    pbr.tiling_y_ = 1.0f;
    pbr.metallic_ = 0.5f;
    pbr.roughness_ = 0.5f;
    pbr.normal_map_intensity_ = 0.0f;
    d.geometry_ = GM.resource_map().getGeometry(geometry_name);
    d.material_.set_type(MaterialType::MT_PBR);
    d.material_.set_data(pbr);
    d.material_.set_albedo(GM.resource_map().getTexture(texture_name));
    d.material_.set_normal(GM.resource_map().getTexture(texture_name));
    d.material_.set_metallic(GM.resource_map().getTexture(texture_name));
    d.material_.set_roughness(GM.resource_map().getTexture(texture_name));
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
    GM.renderer().sun_dir_intensity_.x = x;
    GM.renderer().sun_dir_intensity_.y = y;
    GM.renderer().sun_dir_intensity_.z = z;
}

void leep::SetSunIntensity(float intensity)
{
    GM.renderer().sun_dir_intensity_.w = intensity;
}
