#include "clear.h"

namespace motoret
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

    void Clear::set_clear_color(float r, float g, float b, float a)
    {
        clear_color_[0] = r;
        clear_color_[1] = g;
        clear_color_[2] = b;
        clear_color_[3] = a;
    }

    void Clear::set_clear_buffer(bool color, bool depth, bool stencil)
    {
        color_ = color;
        depth_ = depth;
        stencil_ = stencil;
    }
}

#ifdef MOTORET_OPENGL
    #include "opengl/clear.cpp"
#endif