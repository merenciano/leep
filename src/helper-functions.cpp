#include "helper-functions.h"

#include "core/manager.h"
#include "core/scene-graph.h"
#include "ecs/entity.h"

void leep::CreateRenderable(std::string entity_name)
{
    Entity e = Entity::CreateEntity(entity_name, EntityType::RENDERABLE);
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
    GM.scene_graph().createNode(&eltr, &egtr);
    GM.scene_graph().createNode(&pltr, &pgtr);
    GM.scene_graph().setParent(&eltr, &pltr);
}

void leep::DetachFromParent(std::string entity_name)
{
    LTransform &eltr = Entity::GetEntity(entity_name).getComponent<LTransform>();
    GM.scene_graph().detachFromParent(&eltr);
}

void leep::SetLocation(std::string entity_name, float x, float y, float z)
{
    LTransform &eltr = Entity::GetEntity(entity_name).getComponent<LTransform>();
    eltr.setLocation(glm::vec3(x, y, z));
}
