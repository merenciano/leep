#include "scene.h"
#include "manager.h"

namespace leep {

Scene::Scene()
{
    // Note to myself: Write the next engine in C99
    entities_ = GM.memory().generalAllocT<std::unordered_map<
        EntityType,
        EntityContainer,
        std::hash<EntityType>,
        std::equal_to<EntityType>,
        BuddySTL<std::pair<const EntityType, EntityContainer>>>>(1);
    LEEP_CORE_ASSERT(entities_, "Entities map allocation failed");
}

Scene::~Scene()
{
    // Scenes can be created and deleted several times at runtime
    // so no memory can remain allocated
    entities_->clear();
    GM.memory().generalFree(entities_);
}


EntityContainer &Scene::container(EntityType t)
{
    return entities_->at(t);
}

void Scene::createContainer(EntityType t)
{
    entities_->emplace(std::make_pair(t, t));
}

} // namespace leep