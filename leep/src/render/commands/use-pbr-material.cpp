#include "use-pbr-material.h"

namespace leep
{
    UsePbrMaterial::UsePbrMaterial()
    {

    }

    UsePbrMaterial::~UsePbrMaterial()
    {

    }

    UsePbrMaterial& UsePbrMaterial::set_scene_data(const PbrSceneData &data)
    {
        data_ = data;
        return *this;
    }

    UsePbrMaterial& UsePbrMaterial::set_irradiance_map(Texture t)
    {
        irr_ = t;
        return *this;
    }

    UsePbrMaterial& UsePbrMaterial::set_prefilter_map(Texture t)
    {
        pref_ = t;
        return *this;
    }
    UsePbrMaterial& UsePbrMaterial::set_lut_map(Texture t)
    {
        lut_ = t;
        return *this;
    }
}

#ifdef LEEP_OPENGL
    #include "render/commands/opengl/opengl-use-pbr-material.cpp"
#endif

#ifdef LEEP_OPENGL_ES
    #include "render/commands/opengl/opengl-use-pbr-material.cpp"
#endif