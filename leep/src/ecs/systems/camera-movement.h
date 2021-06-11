#ifndef __LEEP_ECS_SYSTEMS_CAMERA_MOVEMENT_H__
#define __LEEP_ECS_SYSTEMS_CAMERA_MOVEMENT_H__ 1

#include "ecs/system.h"

namespace leep
{
    class CameraMovement : public System
    {
    public:
        CameraMovement();
        CameraMovement(float sensibility, float speed);
        ~CameraMovement() = default;
        virtual void executeSystem() const override;
    private:
        float sensibility_;
        float speed_;
    };
}

#endif // __LEEP_ECS_SYSTEMS_CAMERA_MOVEMENT_H__
