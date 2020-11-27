#include "draw.h"

namespace leep
{
    Draw::Draw()
    {

    }

    Draw::~Draw()
    {

    }

    Draw& Draw::set_geometry(const Geometry &geometry)
    {
        geometry_ = geometry;
        return *this;
    }

    Draw& Draw::set_material(const Material &material)
    {
        material_ = material;
        return *this;
    }
}

#ifdef LEEP_OPENGL
    #include "render/commands/opengl/opengl-draw.cpp"
#endif
