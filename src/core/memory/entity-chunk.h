// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_CORE_MEMORY_ENTITY_CHUNK_H__
#define __LEEP_CORE_MEMORY_ENTITY_CHUNK_H__ 1

#include "ecs/component.h"

#include <stdint.h>
#include <vector>

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

    struct FallingCubeEntities : public EntityChunk
    {
        static const EntityType type = EntityType::FALLING_CUBE;
#ifdef LEEP_DEBUG
        static const uint64_t mask = (1 << COMP_LTRANSFORM) |
                                     (1 << COMP_GTRANSFORM) |
                                     (1 << COMP_DRAWABLE)   |
                                     (1 << COMP_FALL_SPEED) |
                                     (1 << COMP_INFINITE_FALLING_LIMITS);
#endif
        FallingCubeEntities();
        ~FallingCubeEntities();

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

        virtual void relocateLast(EntityChunk *a, uint32_t i) override;
    };
}

#endif // __LEEP_CORE_MEMORY_ENTITY_CHUNK_H__