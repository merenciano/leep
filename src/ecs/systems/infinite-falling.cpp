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

        for (auto &e : GM.entities_)
        {
            if (e.hasComponents(mask))
            {
                Transform *tr = e.getComponent<Transform>();
                InfiniteFallingLimits *ifl = e.getComponent<InfiniteFallingLimits>();

                if (tr->localLocation().y < ifl->limit_down_)
                {
                    float distance = glm::abs(ifl->limit_down_ - ifl->limit_up_) / tr->transform_[1][1];
                    tr->transform_ = glm::translate(tr->transform_, glm::vec3(0.0f, distance, 0.0f));
                }
            }
        }
    }
}