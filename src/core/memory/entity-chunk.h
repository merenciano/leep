// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_CORE_MEMORY_ENTITY_CHUNK_H__
#define __LEEP_CORE_MEMORY_ENTITY_CHUNK_H__ 1

#include "ecs/component.h"
#include "core/common-defs.h"
#include "core/memory/buddy-alloc-stl.h"
#include <stdint.h>
#include <vector>

namespace leep
{
    const uint32_t kMaxEntities = 1000000;
    const uint32_t kMaxChunks = (kMaxEntities / kEntitiesPerChunk) + 1;  // +1 because division rounding
    class EntityChunk 
    {
    public:
        explicit EntityChunk(EntityType t) : type_(t) 
        {
            last_ = 0;
            index_ = -1;
        }
        virtual ~EntityChunk() {}
        EntityType type() const { return type_; }
        virtual void relocateLast(EntityChunk *a, uint32_t i) = 0;
        template<typename C>
        C* component()
        {
            for (auto comp : comps_)
            {
                if (comp->type() == C::s_type)
                {
                    return static_cast<C*>(comp);
                }
            }
            return nullptr;
        }

#ifdef LEEP_DEBUG
        virtual uint64_t mask() const = 0;
#endif

        // ptr to ptr
        std::vector<Component*, BuddySTL<Component*>> comps_;
        const EntityType type_;
        int32_t last_;  // last points to the next unused entity (not a valid one)
        int32_t index_; // index of the chunk in the chunk container
    };

/*
 *      FALLING CUBE ENTITY CHUNK
 */
    struct FallingCubeEntities : public EntityChunk
    {
        static const EntityType s_type = EntityType::FALLING_CUBE;
#ifdef LEEP_DEBUG
        static const uint64_t s_mask = (1 << COMP_LTRANSFORM) |
                                     (1 << COMP_GTRANSFORM) |
                                     (1 << COMP_DRAWABLE)   |
                                     (1 << COMP_FALL_SPEED) |
                                     (1 << COMP_INFINITE_FALLING_LIMITS);
        virtual uint64_t mask() const override { return s_mask; }
#endif
        FallingCubeEntities();
        ~FallingCubeEntities();

        virtual void relocateLast(EntityChunk *a, uint32_t i) override;
    };

/*
 *      RENDERABLE ENTITY CHUNK
 */
    struct RenderableEC : public EntityChunk
    {
        static const EntityType s_type = EntityType::RENDERABLE;
#ifdef LEEP_DEBUG
        static const uint64_t s_mask = (1 << COMP_LTRANSFORM) |
                                     (1 << COMP_GTRANSFORM) |
                                     (1 << COMP_DRAWABLE);
        virtual uint64_t mask() const override { return s_mask; }
#endif
        RenderableEC();
        ~RenderableEC();

        virtual void relocateLast(EntityChunk *a, uint32_t i) override;
    };
}

#endif // __LEEP_CORE_MEMORY_ENTITY_CHUNK_H__