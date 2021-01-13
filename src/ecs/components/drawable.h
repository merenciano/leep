#ifndef __LEEP_ECS_COMPONENTS_DRAWABLE_H__
#define __LEEP_ECS_COMPONENTS_DRAWABLE_H__ 1

#include "ecs/component.h"
#include "render/material.h"
#include "render/geometry.h"

namespace leep 
{
    struct Drawable : public Component
    {
        static const CompType s_type = COMP_DRAWABLE;
        Drawable() : Component(COMP_DRAWABLE) { }
        ~Drawable() {}
        Drawable& operator=(const Drawable &other)
        {
            material_ = other.material_;
            geometry_ = other.geometry_;
            return *this;
        }
        Material material_;
        Geometry geometry_;
    };
}

#endif // __LEEP_ECS_COMPONENTS_DRAWABLE_H__