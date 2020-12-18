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
            uint64_t mask = ((1 << COMP_INFINITE_FALLING_LIMITS) | (1 << COMP_TRANSFORM));
            LEEP_ASSERT((T::mask & mask) == mask, "This container is not valid for this system");

            for (auto &chunk : container_.chunks_)
            {
                for (int32_t i = 0; i < chunk.last_; ++i)
                {
                    glm::mat4 &tr = chunk.transform[i].transform_;
                    float location_y = chunk.transform[i].localLocation().y;
                    float limit_down = chunk.infinite_falling_limits[i].limit_down_;
                    float limit_up = chunk.infinite_falling_limits[i].limit_up_;

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