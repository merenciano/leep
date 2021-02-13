#include "use-framebuffer.h"

#include "core/common-defs.h"

namespace leep
{
    UseFramebuffer::UseFramebuffer()
    {
        default_ = true;
    }

    UseFramebuffer::~UseFramebuffer()
    {

    }

    UseFramebuffer& UseFramebuffer::set_framebuffer(const Framebuffer &fb)
    {
        default_ = false;
        fb_ = fb;
        return *this;
    }
}

#ifdef LEEP_OPENGL
    #include "render/commands/opengl/opengl-use-framebuffer.cpp"
#endif

#ifdef LEEP_OPENGL_ES
    #include "render/commands/opengl/opengl-use-framebuffer.cpp"
#endif