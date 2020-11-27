#ifndef __MOTORET_ECS_SYSTEMS_RENDER_H__
#define __MOTORET_ECS_SYSTEMS_RENDER_H__ 1

#include "ecs/system.h"

namespace motoret
{
    class Render : public System
    {
    public:
        virtual void executeSystem() const override;
    };
}

#endif // __MOTORET_ECS_SYSTEMS_RENDER_H__