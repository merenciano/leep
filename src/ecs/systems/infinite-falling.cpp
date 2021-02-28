#include "infinite-falling.h"

#include "core/memory/entity-container.h"
#include "ecs/components/ltransform.h"
#include "ecs/components/infinite-falling-limits.h"

#include <glm/gtc/matrix_transform.hpp>

namespace leep
{

    void InfiniteFalling::executeSystem() const
    {
#ifdef LEEP_DEBUG
        uint64_t mask = ((1 << COMP_INFINITE_FALLING_LIMITS) | (1 << COMP_LTRANSFORM));
        LEEP_ASSERT((container_.mask() & mask) == mask, "This container is not valid for this system");
#endif
        for (auto &chunk : container_.blocks_)
        {
            LTransform *tr_array = chunk->template component<LTransform>();
            InfiniteFallingLimits *ifl_array = chunk->template component<InfiniteFallingLimits>();
            for (int32_t i = 0; i < chunk->last_; ++i)
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
}
