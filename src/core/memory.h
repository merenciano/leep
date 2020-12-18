// Lucas Merenciano Martinez  <lucasmermar@gmail.com>

#ifndef __LEEP_CORE_MEMORY_H__
#define __LEEP_CORE_MEMORY_H__ 1

#include "core/memory/entity-container.h"

namespace leep
{
    struct Memory
    {
        EntityContainer<FallingCubeEntities> ec_falling_;
    };
}

#endif // __LEEP_CORE_MEMORY_H__