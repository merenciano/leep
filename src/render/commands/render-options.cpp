#include "render/commands/render-options.h"

namespace leep
{
    RenderOptions::RenderOptions()
    {

    }

    RenderOptions::~RenderOptions()
    {

    }

    RenderOptions& RenderOptions::set_blend(BlendFunc sfactor, BlendFunc dfactor)
    {
        s_current_options.sfactor = sfactor;
        s_current_options.dfactor = dfactor;
        return *this;
    }

    RenderOptions& RenderOptions::set_cull_face(CullFace cull)
    {
        s_current_options.cull_face = cull;
        return *this;
    }

    RenderOptions& RenderOptions::set_depth(bool read, bool write)
    {
        s_current_options.read  = read;
        s_current_options.write = write;
        return *this;
    }
}

#ifdef LEEP_OPENGL
    #include "render/commands/opengl/opengl-render-options.cpp"
#endif