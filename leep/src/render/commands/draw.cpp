#include "draw.h"

namespace leep {
Draw::Draw()
{
    inst_count_ = 1;
}

Draw::~Draw()
{
}

Draw& Draw::set_geometry(Geometry geometry)
{
    geometry_ = geometry;
    return *this;
}

Draw& Draw::set_material(Material material)
{
    material_ = material;
    return *this;
}

Draw& Draw::set_instanced_opts(uint32_t inst_count, Buffer inst_attr)
{
    inst_attr_ = inst_attr;
    inst_count_ = inst_count;
    return *this;
}

} // namespace leep

#ifdef LEEP_OPENGL
#include "render/commands/opengl/opengl-draw.cpp"
#endif

#ifdef LEEP_OPENGL_ES
#include "render/commands/opengl/opengl-draw.cpp"
#endif
