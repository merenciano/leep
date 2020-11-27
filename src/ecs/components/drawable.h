#ifndef __LEEP_ECS_COMPONENTS_DRAWABLE_H__
#define __LEEP_ECS_COMPONENTS_DRAWABLE_H__ 1

#include "ecs/component.h"
#include "render/material.h"
#include "render/geometry.h"

namespace leep 
{
    struct Drawable : public Component
    {
        static const CompType type = COMP_DRAWABLE;
        Drawable() : Component(COMP_DRAWABLE) { }
        Material material_;
        Geometry geometry_;
    };
}

#endif // __LEEP_ECS_COMPONENTS_DRAWABLE_H__