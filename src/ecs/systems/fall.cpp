#include "fall.h"

#include "core/manager.h"
#include "core/memory/entity-container.h"
#include "ecs/components/ltransform.h"
#include "ecs/components/fall-speed.h"

#include <glm/gtc/matrix_transform.hpp>

namespace leep
{
    void Fall::executeSystem() const
    {
        float delta = GM.delta_time();
#ifdef LEEP_DEBUG
        uint64_t mask = ((1 << COMP_FALL_SPEED) | (1 << COMP_LTRANSFORM));
        LEEP_ASSERT((container_.mask() & mask) == mask, "This container is not valid for this system.");
#endif
        for (auto &chunk : container_.blocks_)
        {
            LTransform *tr_array = chunk->template component<LTransform>();
            FallSpeed *fs_array = chunk->template component<FallSpeed>();
            for(int32_t i = 0; i < chunk->last_; ++i)
            {
                float fall_speed = fs_array[i].speed_;
                glm::mat4 &tr = tr_array[i].transform_;
                tr = glm::translate(tr, glm::vec3(0.0f, -fall_speed * delta, 0.0f));
            }
        }
    }
}
