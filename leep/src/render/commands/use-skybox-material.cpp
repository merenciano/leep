#include "use-skybox-material.h"

namespace leep
{
    UseSkyboxMaterial::UseSkyboxMaterial()
    {

    }

    UseSkyboxMaterial::~UseSkyboxMaterial()
    {

    }
}

#ifdef LEEP_OPENGL
    #include "render/commands/opengl/opengl-use-skybox-material.cpp"
#endif

#ifdef LEEP_OPENGL_ES
    #include "render/commands/opengl/opengl-use-skybox-material.cpp"
#endif