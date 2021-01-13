// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_ECS_COMPONENTS_GLOBAL_TRANSFORM_H__
#define __LEEP_ECS_COMPONENTS_GLOBAL_TRANSFORM_H__

#include "ecs/component.h"
#include "glm/mat4x4.hpp"

namespace leep 
{
    struct GTransform : public Component
    {
        static const CompType s_type = COMP_GTRANSFORM;
        GTransform() : Component(COMP_GTRANSFORM) { gtr_ = glm::mat4(1.0f); }
        ~GTransform() {}
        GTransform& operator=(const GTransform &gtr) { gtr_ = gtr.gtr_; return *this; }
        glm::vec3 location() const { return glm::vec3(gtr_[3][0], gtr_[3][1], gtr_[3][2]); }
        glm::mat4 gtr_;
    };
}

#endif // __LEEP_ECS_COMPONENTS_GLOBAL_TRANSFORM_H__