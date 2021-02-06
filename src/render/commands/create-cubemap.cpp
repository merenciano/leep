#include "create-cubemap.h"

#include "stb_image.h"

namespace leep
{
    CreateCubemap& CreateCubemap::set_texture(const Texture &texture)
    {
        texture_ = texture;
        return *this;
    }
}

#ifdef LEEP_OPENGL
    #include "render/commands/opengl/opengl-create-cubemap.cpp"
#endif

#ifdef LEEP_OPENGL_ES
    #include "render/commands/opengl/opengl-create-cubemap.cpp"
#endif