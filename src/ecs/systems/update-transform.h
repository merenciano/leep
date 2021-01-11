// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_ECS_SYSTEMS_UPDATE_TRANSFORM_H__
#define __LEEP_ECS_SYSTEMS_UPDATE_TRANSFORM_H__

#include "ecs/system.h"
#include "common-defs.h"
#include "manager.h"

namespace leep
{
    template<typename T>
    class UpdateTransform : public System
    {
    public:
        UpdateTransform() = delete;
        UpdateTransform(EntityContainer<T> &container) : container_(container) {}
        ~UpdateTransform() = default;
        EntityContainer<T> &container_;
        virtual void executeSystem() const override
        {
#ifdef LEEP_DEBUG
            uint64_t mask = ((1 << COMP_LTRANSFORM) | (1 << COMP_GTRANSFORM));
            LEEP_ASSERT((T::mask & mask) == mask, "This container is not valid for this system.");
#endif
            for (auto &chunk : container_.chunks_)
            {
                LTransform *ltr_array = chunk.template component<LTransform>();
                GTransform *gtr_array = chunk.template component<GTransform>();
                std::memcpy(gtr_array, ltr_array, kEntitiesPerChunk*sizeof(LTransform));
            }

            for (auto &chunk : container_.chunks_)
            {
                for(int32_t i = 0; i < chunk.last_; ++i)
                {
                    //gtr_array[i].gtr_ = ltr_array[i].transform_;
                    //GM.scene_graph().
                    TODO: Ver como hacer esto... tiene que ser fuera de este sistema porque
                          los containers deben poder mezclarse en el grafo
                }
            }
        }
    };
}

#endif // __LEEP_ECS_SYSTEMS_UPDATE_TRANSFORM_H__