#include "create-framebuffer.h"

namespace leep
{
    CreateFramebuffer& CreateFramebuffer::set_framebuffer(const Framebuffer &framebuffer)
    {
        framebuffer_ = framebuffer;
        return *this;
    }
}

#ifdef LEEP_OPENGL
    #include "render/commands/opengl/opengl-create-framebuffer.cpp"
#endif

#ifdef LEEP_OPENGL_ES
    #include "render/commands/opengl/opengl-create-framebuffer.cpp"
#endif