#include "use-plain-color-material.h"

namespace leep
{
    UsePlainColorMaterial::UsePlainColorMaterial()
    {

    }

    UsePlainColorMaterial::~UsePlainColorMaterial()
    {

    }
}

#ifdef LEEP_OPENGL
    #include "render/commands/opengl/opengl-use-plain-color-material.cpp"
#endif

#ifdef LEEP_OPENGL_ES
    #include "render/commands/opengl/opengl-use-plain-color-material.cpp"
#endif
