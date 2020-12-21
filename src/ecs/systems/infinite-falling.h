// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_ECS_SYSTEMS_INFINITE_FALLING_H__
#define __LEEP_ECS_SYSTEMS_INFINITE_FALLING_H__ 1

#include "ecs/system.h"

namespace leep
{
    template<typename T>
    class InfiniteFalling : public System
    {
    public:
        InfiniteFalling() = delete;
        InfiniteFalling(EntityContainer<T> &container) : container_(container) {}
        EntityContainer<T> &container_;
        virtual void executeSystem() const override
        {
#ifdef LEEP_DEBUG
            uint64_t mask = ((1 << COMP_INFINITE_FALLING_LIMITS) | (1 << COMP_TRANSFORM));
            LEEP_ASSERT((T::mask & mask) == mask, "This container is not valid for this system");
#endif
            for (auto &chunk : container_.chunks_)
            {
                Transform *tr_array = chunk.template component<Transform>();
                InfiniteFallingLimits *ifl_array = chunk.template component<InfiniteFallingLimits>();
                for (int32_t i = 0; i < chunk.last_; ++i)
                {
                    glm::mat4 &tr = tr_array[i].transform_;
                    float location_y = tr_array[i].localLocation().y;
                    float limit_down = ifl_array[i].limit_down_;
                    float limit_up = ifl_array[i].limit_up_;

                    if (location_y < limit_down)
                    {
                        float distance = glm::abs(limit_down - limit_up) / tr[1][1];
                        tr = glm::translate(tr, glm::vec3(0.0f, distance, 0.0f));
                    }
                }
            }
        }
    };
}

#endif // __LEEP_ECS_SYSTEMS_INFINITE_FALLING_H__