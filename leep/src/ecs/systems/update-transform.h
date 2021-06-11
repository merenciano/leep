// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_ECS_SYSTEMS_UPDATE_TRANSFORM_H__
#define __LEEP_ECS_SYSTEMS_UPDATE_TRANSFORM_H__

#include "ecs/system.h"
#include "core/common-defs.h"
#include "core/manager.h"

namespace leep
{
    class UpdateTransform : public System
    {
    public:
        UpdateTransform() = delete;
        UpdateTransform(class EntityContainer &container) : container_(container) {}
        ~UpdateTransform() = default;
        class EntityContainer &container_;
        virtual void executeSystem() const override;
    };
}

#endif // __LEEP_ECS_SYSTEMS_UPDATE_TRANSFORM_H__
