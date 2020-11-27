#ifndef __MOTORET_ECS_COMPONENTS_TRANSFORM_H__
#define __MOTORET_ECS_COMPONENTS_TRANSFORM_H__ 1

#include "ecs/component.h"
#include "glm/mat4x4.hpp"

namespace motoret
{
    struct Transform : public Component
    {
        static const CompType type = COMP_TRANSFORM;
        Transform() : Component(COMP_TRANSFORM) { transform_ = glm::mat4(1.0f); }
        glm::mat4 transform_;
    };
}

#endif // __MOTORET_ECS_COMPONENTS_TRANSFORM_H__