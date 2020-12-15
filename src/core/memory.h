// Lucas Merenciano Martinez  <lucasmermar@gmail.com>

#ifndef __LEEP_CORE_MEMORY_H__
#define __LEEP_CORE_MEMORY_H__ 1

#include "ecs/components/drawable.h"
#include "ecs/components/transform.h"
#include "ecs/components/fall-speed.h"
#include "ecs/components/infinite-falling-limits.h"

namespace leep
{
    namespace memory
    {
        // Entity Chunk
        struct FallingCubeEntities
        {
            uint64_t mask = (1 << COMP_TRANSFORM) | (1 << COMP_DRAWABLE) | (1 << COMP_FALL_SPEED) | (1 << COMP_INFINITE_FALLING_LIMITS);
            //bool hasComponents(uint64_t comp_mask) { return (comp_mask & mask) == comp_mask; }
            Transform               transform               [10000];
            Drawable                drawable                [10000];
            FallSpeed               fall_speed              [10000];
            InfiniteFallingLimits   infinite_falling_limits [10000];
        };

        struct StackMemory
        {
            FallingCubeEntities falling_cube_entities_;
        };
    }
}

#endif // __LEEP_CORE_MEMORY_H__