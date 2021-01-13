// Lucas Merenciano Martinez  <lucasmermar@gmail.com>

#ifndef __LEEP_CORE_MEMORY_H__
#define __LEEP_CORE_MEMORY_H__ 1

#include "core/memory/entity-container.h"

#include <unordered_map>

namespace leep
{
    class Memory
    {
    public:
        Memory();

        EntityContainer& container(EntityType t);
        EntityContainer ec_falling_ = EntityContainer(EntityType::FALLING_CUBE);
        EntityContainer ec_renderable_ = EntityContainer(EntityType::RENDERABLE);
        std::unordered_map<EntityType, EntityContainer> entities_;
    };
}

#endif // __LEEP_CORE_MEMORY_H__