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

        for (auto &e : GM.entities_)
        {
            if (e.hasComponents(mask))
            {
                Transform *tr = e.getComponent<Transform>();
                FallSpeed *fs = e.getComponent<FallSpeed>();

                tr->transform_ = glm::translate(tr->transform_, glm::vec3(0.0f, -(fs->speed_), 0.0f));
            }
        }
    }
}