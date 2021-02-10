// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_ECS_SYSTEMS_FALL_H__
#define __LEEP_ECS_SYSTEMS_FALL_H__ 1

#include "ecs/system.h"

namespace leep
{
    class Fall : public System
    {
    public:
        Fall() = delete;
        Fall(class EntityContainer &container) : container_(container) {}
        class EntityContainer &container_;
        virtual void executeSystem() const override;
    };
}

#endif // __LEEP_ECS_SYSTEMS_FALL_H__
