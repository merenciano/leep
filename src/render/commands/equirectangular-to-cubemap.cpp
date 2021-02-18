#include "equirectangular-to-cubemap.h"
#include "core/common-defs.h"
#include "core/manager.h"
#include "render/renderer.h"

namespace leep
{
    EquirectangularToCubemap::EquirectangularToCubemap()
    {

    }

    EquirectangularToCubemap::~EquirectangularToCubemap()
    {

    }
/*
    EquirectangularToCubemap& EquirectangularToCubemap::set_in_equirect(Texture t)
    {
        LEEP_CORE_ASSERT(t.handle() != CommonDefs::UNINIT_HANDLE,
            "Texture not created");
        LEEP_CORE_ASSERT(GM.renderer().textures_[t.handle()].type_ == TexType::RGB_F16, "Equirect must be HDR");
        in_equi_ = t;
        return *this;
    }
    */

    EquirectangularToCubemap& EquirectangularToCubemap::set_out_cube(Texture t)
    {
        LEEP_CORE_ASSERT(t.handle() != CommonDefs::UNINIT_HANDLE,
            "Texture not created");
        LEEP_CORE_ASSERT(GM.renderer().textures_[t.handle()].type_ == TexType::ENVIRONMENT,
            "Cubemap must be Environment");
        out_cube_ = t;
        return *this;
    }
}

#ifdef LEEP_OPENGL
    #include "render/commands/opengl/opengl-equirectangular-to-cubemap.cpp"
#endif

#ifdef LEEP_OPENGL_ES
    #include "render/commands/opengl/opengl-equirectangular-to-cubemap.cpp"
#endif
