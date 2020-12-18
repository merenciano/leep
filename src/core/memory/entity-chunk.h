// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_CORE_MEMORY_ENTITY_CHUNK_H__
#define __LEEP_CORE_MEMORY_ENTITY_CHUNK_H__ 1

#include "ecs/components/drawable.h"
#include "ecs/components/transform.h"
#include "ecs/components/fall-speed.h"
#include "ecs/components/infinite-falling-limits.h"

#include <stdint.h>

namespace leep
{
    const uint32_t kEntitiesPerChunk = 128; 

    enum class EntityType
    {
        NONE = 0,
        STANDARD,
        FALLING_CUBE,
    };

    class EntityChunk 
    {
    public:
        explicit EntityChunk(EntityType t) : type_(t) { last_ = 0; index_ = -1; }
        ~EntityChunk() = default;
        EntityType type() const { return type_; }
        virtual void relocateLast(EntityChunk *a, int32_t i) = 0;

        const EntityType type_;
        //const uint64_t   mask_;
        int32_t last_;  // last points to the next unused entity (not a valid one)
        int32_t index_; // index of the chunk in the chunk container
    };

    struct FallingCubeEntities : EntityChunk
    {
        static const EntityType type = EntityType::FALLING_CUBE;
        static const uint64_t mask = (1 << COMP_TRANSFORM)  |
                                     (1 << COMP_DRAWABLE)   |
                                     (1 << COMP_FALL_SPEED) |
                                     (1 << COMP_INFINITE_FALLING_LIMITS);
        FallingCubeEntities() : EntityChunk(type) {}

        virtual void relocateLast(EntityChunk *a, int32_t i) override
        {
            LEEP_ASSERT(a && i < kEntitiesPerChunk, "Wrong parameters");
            FallingCubeEntities *chunk = static_cast<FallingCubeEntities*>(a);
            chunk->transform[i] = transform[last_ - 1];
            chunk->drawable[i] = drawable[last_ - 1];
            chunk->fall_speed[i] = fall_speed[last_ - 1];
            chunk->infinite_falling_limits[i] = infinite_falling_limits[last_ - 1];
#ifdef LEEP_DEBUG
            transform[last_ - 1] = Transform();
            drawable[last_ - 1] = Drawable();
            fall_speed[last_ - 1] = FallSpeed(); 
            infinite_falling_limits[last_ - 1] = InfiniteFallingLimits();
#endif
            last_--;
        }

        Transform               transform               [kEntitiesPerChunk];
        Drawable                drawable                [kEntitiesPerChunk];
        FallSpeed               fall_speed              [kEntitiesPerChunk];
        InfiniteFallingLimits   infinite_falling_limits [kEntitiesPerChunk];
    };
}

#endif // __LEEP_CORE_MEMORY_ENTITY_CHUNK_H__