#include "clear.h"

namespace leep 
{
    Clear::Clear()
    {
        clear_color_[0] = 0.0f;
        clear_color_[1] = 0.0f;
        clear_color_[2] = 0.0f;
        clear_color_[3] = 1.0f;

        color_ = true;
        depth_ = true;
        stencil_ = true;
    }

    Clear::~Clear()
    {

    }

    Clear& Clear::set_clear_color(float r, float g, float b, float a)
    {
        clear_color_[0] = r;
        clear_color_[1] = g;
        clear_color_[2] = b;
        clear_color_[3] = a;
        return *this;
    }

    Clear& Clear::set_clear_buffer(bool color, bool depth, bool stencil)
    {
        color_ = color;
        depth_ = depth;
        stencil_ = stencil;
        return *this;
    }
}

#ifdef LEEP_OPENGL
    #include "render/commands/opengl/opengl-clear.cpp"
#endif

#ifdef LEEP_OPENGL_ES
    #include "render/commands/opengl/opengl-clear.cpp"
#endif
