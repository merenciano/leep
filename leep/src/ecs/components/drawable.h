#ifndef __LEEP_ECS_COMPONENTS_DRAWABLE_H__
#define __LEEP_ECS_COMPONENTS_DRAWABLE_H__ 1

#include "ecs/component.h"
#include "render/Cmaterial.h"

namespace leep 
{
    struct Drawable : public Component
    {
        static const CompType s_type = COMP_DRAWABLE;
        Drawable() : Component(COMP_DRAWABLE) {  }

        Drawable& operator=(const Drawable &other)
        {
            mat = other.mat;
            mesh = other.mesh;
            return *this;
        }
        THE_Material mat;
        THE_Mesh mesh;
    };
}

#endif // __LEEP_ECS_COMPONENTS_DRAWABLE_H__