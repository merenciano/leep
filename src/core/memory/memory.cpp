#include "memory.h"

namespace leep
{
    Memory::Memory()
    {
    }

    EntityContainer& Memory::container(EntityType t)
    {
        return entities_[t];
    }
}