// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_ECS_SYSTEMS_RENDER_H__
#define __LEEP_ECS_SYSTEMS_RENDER_H__ 1

#include "ecs/system.h"

namespace leep
{
    class Render : public System
    {
    public:
        virtual void executeSystem() const override;
    };
}

#endif // __LEEP_ECS_SYSTEMS_RENDER_H__