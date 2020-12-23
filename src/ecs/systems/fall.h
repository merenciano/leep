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
#ifdef LEEP_DEBUG
            uint64_t mask = ((1 << COMP_FALL_SPEED) | (1 << COMP_TRANSFORM));
            LEEP_ASSERT((T::mask & mask) == mask, "This container is not valid for this system.");
#endif
            for (auto &chunk : container_.chunks_)
            {
                Transform *tr_array = chunk.template component<Transform>();
                FallSpeed *fs_array = chunk.template component<FallSpeed>();
                for(int32_t i = 0; i < chunk.last_; ++i)
                {
                    float fall_speed = fs_array[i].speed_;
                    glm::mat4 &tr = tr_array[i].transform_;
                    tr = glm::translate(tr, glm::vec3(0.0f, -fall_speed, 0.0f));
                }
            }
        }
    };
}

#endif // __LEEP_ECS_SYSTEMS_FALL_H__