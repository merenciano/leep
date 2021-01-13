// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_ECS_COMPONENTS_FALL_SPEED_H__
#define __LEEP_ECS_COMPONENTS_FALL_SPEED_H__ 1

#include "ecs/component.h"

namespace leep 
{
    struct FallSpeed : public Component
    {
        static const CompType s_type = COMP_FALL_SPEED;
        FallSpeed() : Component(COMP_FALL_SPEED) { speed_ = 1.0f; }
        FallSpeed& operator=(const FallSpeed &other) { speed_ = other.speed_; return *this; }
        float speed_;
    };
}

#endif // __LEEP_ECS_COMPONENTS_FALL_SPEED_H__
