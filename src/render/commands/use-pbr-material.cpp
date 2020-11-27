#include "use-pbr-material.h"

namespace leep
{
    UsePbrMaterial::UsePbrMaterial()
    {

    }

    UsePbrMaterial::~UsePbrMaterial()
    {

    }

    UsePbrMaterial& UsePbrMaterial::set_data(const PbrSceneData &data)
    {
        data_ = data;
        return *this;
    }
}

#ifdef LEEP_OPENGL
    #include "render/commands/opengl/opengl-use-pbr-material.cpp"
#endif