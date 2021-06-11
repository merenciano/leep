// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_ECS_COMPONENTS_INFINITE_FALLING_LIMITS_H__
#define __LEEP_ECS_COMPONENTS_INFINITE_FALLING_LIMITS_H__ 1

#include "ecs/component.h"

namespace leep 
{
    struct InfiniteFallingLimits : public Component
    {
        static const CompType s_type = COMP_INFINITE_FALLING_LIMITS;
        InfiniteFallingLimits() : Component(COMP_INFINITE_FALLING_LIMITS) { limit_up_ = 1.0f; limit_down_ = -1.0f; }
        InfiniteFallingLimits& operator=(const InfiniteFallingLimits &other)
        {
            limit_up_ = other.limit_up_;
            limit_down_ = other.limit_down_;
            return *this;
        }
        float limit_up_;
        float limit_down_;
    };
}

#endif // __LEEP_ECS_COMPONENTS_INFINITE_FALLING_LIMITS_H__
