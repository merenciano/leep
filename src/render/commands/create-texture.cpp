#include "create-texture.h"

namespace leep
{
	CreateTexture::CreateTexture()
	{
        release_ram_data_ = true;
	}

    CreateTexture& CreateTexture::set_texture(const Texture &texture)
    {
        texture_ = texture;
        return *this;
    }

    CreateTexture& CreateTexture::releaseRamData(bool value)
    {
        release_ram_data_ = value;
        return *this;
    }
}

#ifdef LEEP_OPENGL
    #include "render/commands/opengl/opengl-create-texture.cpp"
#endif

#ifdef LEEP_OPENGL_ES
    #include "render/commands/opengl/opengl-create-texture.cpp"
#endif