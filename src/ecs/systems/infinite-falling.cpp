#include "infinite-falling.h"
#include "core/common-defs.h"
#include "core/manager.h"
#include "ecs/components/transform.h"
#include "ecs/components/infinite-falling-limits.h"

#include "glm/gtc/matrix_transform.hpp"

namespace leep 
{
    void InfiniteFalling::executeSystem() const
    {
        uint64_t mask = ((1 << COMP_INFINITE_FALLING_LIMITS) | (1 << COMP_TRANSFORM));
        memory::FallingCubeEntities &entity_chunk = GM.stack_memory_.falling_cube_entities_;

        if ((mask & entity_chunk.mask) == mask)
        {
            for (int32_t i = 0; i < 10000; ++i)
            {
                glm::mat4 &tr = entity_chunk.transform[i].transform_;
                float location_y = entity_chunk.transform[i].localLocation().y;
                float limit_down = entity_chunk.infinite_falling_limits[i].limit_down_;
                float limit_up = entity_chunk.infinite_falling_limits[i].limit_up_;

                if (location_y < limit_down)
                {
                    float distance = glm::abs(limit_down - limit_up) / tr[1][1];
                    tr = glm::translate(tr, glm::vec3(0.0f, distance, 0.0f));
                }
            }
        }
    }
}