// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_ECS_SYSTEMS_UPDATE_TRANSFORM_H__
#define __LEEP_ECS_SYSTEMS_UPDATE_TRANSFORM_H__

#include "ecs/system.h"
#include "core/common-defs.h"
#include "core/manager.h"

namespace leep
{
    class UpdateTransform : public System
    {
    public:
        UpdateTransform() = delete;
        UpdateTransform(EntityContainer &container) : container_(container) {}
        ~UpdateTransform() = default;
        EntityContainer &container_;
        virtual void executeSystem() const override
        {
            for (auto &chunk : container_.chunks_)
            {
                LTransform *ltr_array;
                GTransform *gtr_array;
                switch(container_.type())
                {
                    case EntityType::FALLING_CUBE:
#ifdef LEEP_DEBUG
                        uint64_t mask = ((1 << COMP_LTRANSFORM) | (1 << COMP_GTRANSFORM));
                        LEEP_ASSERT((FallingCubeEntities::mask & mask) == mask, "This container is not valid for this system.");
#endif
                        ltr_array = static_cast<FallingCubeEntities*>(chunk)
                            ->template component<LTransform>();
                        gtr_array = static_cast<FallingCubeEntities*>(chunk)
                            ->template component<GTransform>();
                        break;
                }
                std::memcpy((void*)gtr_array, ltr_array, kEntitiesPerChunk*sizeof(LTransform));
            }
        }
    };
}

#endif // __LEEP_ECS_SYSTEMS_UPDATE_TRANSFORM_H__