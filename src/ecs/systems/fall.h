// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_ECS_SYSTEMS_FALL_H__
#define __LEEP_ECS_SYSTEMS_FALL_H__ 1

#include "ecs/system.h"

namespace leep
{
    template <typename T>
    class Fall : public System
    {
    public:
        Fall() = delete;
        Fall(EntityContainer<T> &container) : container_(container) {}
        EntityContainer<T> &container_;
        virtual void executeSystem() const override
        {
            uint64_t mask = ((1 << COMP_FALL_SPEED) | (1 << COMP_TRANSFORM));
            LEEP_ASSERT((T::mask & mask) == mask, "This container is not valid for this system.");

            for (auto &chunk : container_.chunks_)
            {
                for(int32_t i = 0; i < chunk.last_; ++i)
                {
                    float fall_speed = chunk.fall_speed[i].speed_;
                    glm::mat4 &tr = chunk.transform[i].transform_;
                    tr = glm::translate(tr, glm::vec3(0.0f, -fall_speed, 0.0f));
                }
            }
        }
    };
}

#endif // __LEEP_ECS_SYSTEMS_FALL_H__