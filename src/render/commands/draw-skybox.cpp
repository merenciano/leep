#include "draw-skybox.h"

namespace leep
{
    DrawSkybox::DrawSkybox()
    {
        material_.set_type(MT_SKYBOX);
    }

    DrawSkybox::~DrawSkybox()
    {

    }

    DrawSkybox& DrawSkybox::set_cubemap(const Texture &cubemap)
    {
        material_.set_albedo(cubemap);
        return *this;
    }
}

#ifdef LEEP_OPENGL
    #include "render/commands/opengl/opengl-draw-skybox.cpp"
#endif

#ifdef LEEP_OPENGL_ES
    #include "render/commands/opengl/opengl-draw-skybox.cpp"
#endif
