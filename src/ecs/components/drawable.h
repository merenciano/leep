#ifndef __MOTORET_ECS_COMPONENTS_DRAWABLE_H__
#define __MOTORET_ECS_COMPONENTS_DRAWABLE_H__ 1

#include "ecs/component.h"
#include "render/material.h"
#include "render/geometry.h"

namespace motoret
{
    struct Drawable : public Component
    {
        static const CompType type = COMP_DRAWABLE;
        Drawable() : Component(COMP_DRAWABLE) { }
        Material material_;
        Geometry geometry_;
    };
}

#endif // __MOTORET_ECS_COMPONENTS_DRAWABLE_H__