// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_ECS_COMPONENTS_LOCAL_TRANSFORM_H__
#define __LEEP_ECS_COMPONENTS_LOCAL_TRANSFORM_H__

#include "ecs/component.h"
#include "core/common-defs.h"
#include "core/Cdefinitions.h"

namespace leep 
{
    struct LTransform : public Component
    {
        static const CompType s_type = COMP_LTRANSFORM;
	LTransform() : Component(COMP_LTRANSFORM) { transform_ = glm::mat4(1.0f); transform = smat4_identity(); }
        ~LTransform() {}
        LTransform& operator=(const LTransform &tr);
        LTransform& rotateYWorld(float radians);
	static struct mat4 rotateYWorld(struct mat4 tr, float radians);
        LTransform& translate(float x, float y, float z);
        glm::vec3 localLocation() const;
        void setLocation(glm::vec3 new_location);
        glm::mat4 transform_;
	struct mat4 transform;
    };
}

#endif // __LEEP_ECS_COMPONENTS_LOCAL_TRANSFORM_H__