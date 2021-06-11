// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_ECS_SYSTEMS_DELETE_RELEASED_H__
#define __LEEP_ECS_SYSTEMS_DELETE_RELEASED_H__

#include "ecs/system.h"

namespace leep
{
    // Mark for deletion one texture and one buffer each time is called
    // if there is any released one
    // In case that more than one is released it will take more frames
    class DeleteReleased: public System
    {
    public:
        DeleteReleased() {}
        virtual void executeSystem() const override;
    };
}

#endif // __LEEP_ECS_SYSTEMS_DELETE_RELEASED_H__
