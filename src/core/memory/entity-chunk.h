// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_CORE_MEMORY_ENTITY_CHUNK_H__
#define __LEEP_CORE_MEMORY_ENTITY_CHUNK_H__ 1

#include "core/common-defs.h"
#include "ecs/components/drawable.h"
#include "ecs/components/ltransform.h"
#include "ecs/components/gtransform.h"
#include "ecs/components/fall-speed.h"
#include "ecs/components/infinite-falling-limits.h"

#include <stdint.h>

namespace leep
{
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
        virtual ~EntityChunk() {}
        EntityType type() const { return type_; }
        virtual void relocateLast(EntityChunk *a, uint32_t i) = 0;

        std::vector<Component*> comps_;
        const EntityType type_;
        int32_t last_;  // last points to the next unused entity (not a valid one)
        int32_t index_; // index of the chunk in the chunk container
    };

    struct FallingCubeEntities : EntityChunk
    {
        static const EntityType type = EntityType::FALLING_CUBE;
#ifdef LEEP_DEBUG
        static const uint64_t mask = (1 << COMP_LTRANSFORM) |
                                     (1 << COMP_GTRANSFORM) |
                                     (1 << COMP_DRAWABLE)   |
                                     (1 << COMP_FALL_SPEED) |
                                     (1 << COMP_INFINITE_FALLING_LIMITS);
#endif
        FallingCubeEntities() : EntityChunk(type) 
        {
            comps_.emplace_back(new LTransform[kEntitiesPerChunk]());
            comps_.emplace_back(new Drawable[kEntitiesPerChunk]());
            comps_.emplace_back(new FallSpeed[kEntitiesPerChunk]());
            comps_.emplace_back(new InfiniteFallingLimits[kEntitiesPerChunk]());
            comps_.emplace_back(new GTransform[kEntitiesPerChunk]());
        }

        ~FallingCubeEntities()
        {
            delete[] static_cast<LTransform*>(comps_[0]);
            delete[] static_cast<Drawable*>(comps_[1]);
            delete[] static_cast<FallSpeed*>(comps_[2]);
            delete[] static_cast<InfiniteFallingLimits*>(comps_[3]);
            delete[] static_cast<GTransform*>(comps_[4]);
        }

        template<typename C>
        C* component()
        {
            switch(C::type)
            {
                case COMP_LTRANSFORM: return static_cast<C*>(comps_[0]);
                case COMP_DRAWABLE: return static_cast<C*>(comps_[1]);
                case COMP_FALL_SPEED: return static_cast<C*>(comps_[2]);
                case COMP_INFINITE_FALLING_LIMITS: return static_cast<C*>(comps_[3]);
                case COMP_GTRANSFORM: return static_cast<C*>(comps_[4]);
            }
        }

        virtual void relocateLast(EntityChunk *a, uint32_t i) override
        {
            LEEP_ASSERT(a && i < kEntitiesPerChunk, "Wrong parameters");
            FallingCubeEntities *chunk = static_cast<FallingCubeEntities*>(a);
            static_cast<LTransform*>(chunk->comps_[0])[i] = static_cast<LTransform*>(comps_[0])[last_ - 1];
            static_cast<Drawable*>(chunk->comps_[1])[i] = static_cast<Drawable*>(comps_[1])[last_ - 1];
            static_cast<FallSpeed*>(chunk->comps_[2])[i] = static_cast<FallSpeed*>(comps_[2])[last_ - 1];
            static_cast<InfiniteFallingLimits*>(chunk->comps_[3])[i] = static_cast<InfiniteFallingLimits*>(comps_[3])[last_ - 1];
            static_cast<GTransform*>(chunk->comps_[4])[i] = static_cast<GTransform*>(comps_[4])[last_ - 1];
#ifdef LEEP_DEBUG
            static_cast<LTransform*>(comps_[0])[last_ - 1] = LTransform();
            static_cast<Drawable*>(comps_[1])[last_ - 1] = Drawable();
            static_cast<FallSpeed*>(comps_[2])[last_ - 1] = FallSpeed();
            static_cast<InfiniteFallingLimits*>(comps_[3])[last_ - 1] = InfiniteFallingLimits();
            static_cast<GTransform*>(comps_[4])[last_ - 1] = GTransform();
#endif
        }
    };
}

#endif // __LEEP_CORE_MEMORY_ENTITY_CHUNK_H__