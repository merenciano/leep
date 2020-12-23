// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_ECS_COMPONENTS_TRANSFORM_H__
#define __LEEP_ECS_COMPONENTS_TRANSFORM_H__ 1

#include "ecs/component.h"
#include "glm/mat4x4.hpp"

namespace leep 
{
    struct Transform : public Component
    {
        static const CompType type = COMP_TRANSFORM;
        Transform() : Component(COMP_TRANSFORM) { transform_ = glm::mat4(1.0f); }
        ~Transform() {}
        Transform& operator=(const Transform &tr);
        Transform& rotateYWorld(float radians);
        glm::vec3 localLocation() const;
        glm::mat4 transform_;
    };
}

#endif // __LEEP_ECS_COMPONENTS_TRANSFORM_H__