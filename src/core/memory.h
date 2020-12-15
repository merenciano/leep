// Lucas Merenciano Martinez  <lucasmermar@gmail.com>

#ifndef __LEEP_CORE_MEMORY_H__
#define __LEEP_CORE_MEMORY_H__ 1

#include "ecs/components/drawable.h"
#include "ecs/components/transform.h"
#include "ecs/components/fall-speed.h"
#include "ecs/components/infinite-falling-limits.h"

namespace leep
{
    const uint32_t kEntitiesPerChunk = 10000; 
    namespace memory
    {
        // Entity Chunk
        struct FallingCubeEntities
        {
            uint64_t mask = (1 << COMP_TRANSFORM) | (1 << COMP_DRAWABLE) | (1 << COMP_FALL_SPEED) | (1 << COMP_INFINITE_FALLING_LIMITS);
            //bool hasComponents(uint64_t comp_mask) { return (comp_mask & mask) == comp_mask; }
            Transform               transform               [kEntitiesPerChunk];
            Drawable                drawable                [kEntitiesPerChunk];
            FallSpeed               fall_speed              [kEntitiesPerChunk];
            InfiniteFallingLimits   infinite_falling_limits [kEntitiesPerChunk];
        };

        struct StackMemory
        {
            FallingCubeEntities falling_cube_entities_;
        };
    }
}

#endif // __LEEP_CORE_MEMORY_H__