#include "memory.h"

namespace leep
{
    Memory::Memory()
    {
        renderq_.init(kRenderQueueSize);
    }

    EntityContainer& Memory::container(EntityType t)
    {
        return entities_.at(t);
    }

    void Memory::createContainer(EntityType t)
    {
        entities_.emplace(std::make_pair(t, t));
    }
}