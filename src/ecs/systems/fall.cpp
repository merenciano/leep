#include "fall.h"
#include "core/common-defs.h"
#include "core/manager.h"
#include "ecs/components/transform.h"
#include "ecs/components/fall-speed.h"

#include "glm/gtc/matrix_transform.hpp"

namespace leep 
{
    void Fall::executeSystem() const
    {
        uint64_t mask = ((1 << COMP_FALL_SPEED) | (1 << COMP_TRANSFORM));
        memory::FallingCubeEntities &entity_chunk = GM.stack_memory_.falling_cube_entities_;

        if ((mask & entity_chunk.mask) == mask) 
        {
            for(int32_t i = 0; i < 10000; ++i)
            {
                float fall_speed = entity_chunk.fall_speed[i].speed_;
                glm::mat4 &tr = entity_chunk.transform[i].transform_;
                tr = glm::translate(tr, glm::vec3(0.0f, -fall_speed, 0.0f));
            }
        }
    }
}