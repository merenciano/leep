#include "create-framebuffer.h"

#include "core/common-defs.h"

namespace leep
{
    CreateFramebuffer& CreateFramebuffer::set_framebuffer(const Framebuffer &fb)
    {
        LEEP_CHECK_RESOURCE(fb);
        fb_ = fb;
        return *this;
    }
}

#ifdef LEEP_OPENGL
    #include "render/commands/opengl/opengl-create-framebuffer.cpp"
#endif

#ifdef LEEP_OPENGL_ES
    #include "render/commands/opengl/opengl-create-framebuffer.cpp"
#endif