// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_ECS_SYSTEMS_FALL_H__
#define __LEEP_ECS_SYSTEMS_FALL_H__ 1

#include "ecs/system.h"

namespace leep
{
    class Fall : public System
    {
    public:
        Fall() = delete;
        Fall(EntityContainer &container) : container_(container) {}
        EntityContainer &container_;
        virtual void executeSystem() const override
        {
            for (auto &chunk : container_.chunks_)
            {
                LTransform *tr_array;
                FallSpeed *fs_array;
                switch(container_.type())
                {
                    case EntityType::FALLING_CUBE:
#ifdef LEEP_DEBUG
                        uint64_t mask = ((1 << COMP_FALL_SPEED) | (1 << COMP_LTRANSFORM));
                        LEEP_ASSERT((FallingCubeEntities::mask & mask) == mask, "This container is not valid for this system.");
#endif
                        tr_array = static_cast<FallingCubeEntities*>(chunk)
                            ->template component<LTransform>();
                        fs_array = static_cast<FallingCubeEntities*>(chunk)
                            ->template component<FallSpeed>();
                        break;
                }
                for(int32_t i = 0; i < chunk->last_; ++i)
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