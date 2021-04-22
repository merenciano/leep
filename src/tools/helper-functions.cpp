#include "helper-functions.h"

#include "core/manager.h"
#include "core/scene.h"
#include "ecs/entity.h"
#include "ecs/components/drawable.h"
#include "tools/resource-map.h"

void leep::CreateRenderable(std::string entity_name,
                            std::string geometry_name,
                            std::string texture_name)
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
    pbr.tiling_x_ = 1.0f;
    pbr.tiling_y_ = 1.0f;
    pbr.metallic_ = 0.0f;
    pbr.roughness_ = 0.0f;
    pbr.reflectance_ = 5.0f;
    d.geometry_ = GM.resource_map().getGeometry(geometry_name);
    d.material_.set_type(MaterialType::MT_PBR);
    d.material_.set_data(pbr);
    d.material_.set_albedo(GM.resource_map().getTexture(texture_name));
}

void leep::RemoveEntity(std::string entity_name)
{
    Entity::RemoveEntity(entity_name);
}

void leep::SetParent(std::string entity_name, std::string parent_name)
{
    LTransform &eltr = Entity::GetEntity(entity_name).getComponent<LTransform>();
    LTransform &pltr = Entity::GetEntity(parent_name).getComponent<LTransform>();
    GTransform &egtr = Entity::GetEntity(entity_name).getComponent<GTransform>();
    GTransform &pgtr = Entity::GetEntity(parent_name).getComponent<GTransform>();
    GM.scene().scene_graph_.createNode(&eltr, &egtr);
    GM.scene().scene_graph_.createNode(&pltr, &pgtr);
    GM.scene().scene_graph_.setParent(&eltr, &pltr);
}

void leep::DetachFromParent(std::string entity_name)
{
    LTransform &eltr = Entity::GetEntity(entity_name).getComponent<LTransform>();
    GM.scene().scene_graph_.detachFromParent(&eltr);
}

void leep::SetLocation(std::string entity_name, float x, float y, float z)
{
    LTransform &eltr = Entity::GetEntity(entity_name).getComponent<LTransform>();
    eltr.setLocation(glm::vec3(x, y, z));
}
