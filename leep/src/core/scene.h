// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_CORE_SCENE_H__
#define __LEEP_CORE_SCENE_H__

#include "core/scene-graph.h"
#include "core/memory/entity-container.h"
#include "core/memory/buddy-alloc-stl.h"
#include "ecs/entity-map.h"

#include <unordered_map>

namespace leep
{
    class Scene
    {
    public:
        Scene();
        ~Scene();

        void createContainer(EntityType t);
        EntityContainer& container(EntityType t);
        std::unordered_map<
            EntityType,
            EntityContainer,
            std::hash<EntityType>,
            std::equal_to<EntityType>,
            BuddySTL<std::pair<const EntityType, EntityContainer>>> *entities_;

        SceneGraph scene_graph_;
        EntityMap entity_map_;
    };
}
#endif // __LEEP_CORE_SCENE_H__