#include "entity.h"

namespace motoret
{
    Entity Entity::CreateEntity(std::string name)
    {
        InternalEntity ie(name);
        int32_t index = 0;

        MOTORET_CORE_ASSERT(GM.entity_map_.find(name) == GM.entity_map_.end(), "An entity with that name already exists.");
        GM.entities_.push_back(std::move(ie));
        MOTORET_CORE_ASSERT(!GM.entities_.empty(), "Entity creation error.");
        index = GM.entities_.size() - 1;
        GM.entity_map_[name] = index;
        return Entity(index);
    }

    void Entity::RemoveEntity(std::string name)
    {
        if (GM.entity_map_.find(name) == GM.entity_map_.end())
        {
            // Entity already removed or never created
            // in any case the job is done
            return;
        }

        int32_t index = GM.entity_map_[name];
        GM.entity_map_.erase(name);
        // Swap the last entity with the removed one
        GM.entities_[index].swap(&(*GM.entities_.rbegin()));
        // Now the entity to be removed is in the last position
        GM.entities_.pop_back();
        // Set the correct index for the swapped entity
        GM.entity_map_[GM.entities_[index].name_] = index;
    }
}