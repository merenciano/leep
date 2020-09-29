#include "use-plain-color-material.h"

namespace motoret
{
    UsePlainColorMaterial::UsePlainColorMaterial()
    {

    }

    UsePlainColorMaterial::~UsePlainColorMaterial()
    {

    }
}

#ifdef MOTORET_OPENGL
    #include "render/commands/opengl/opengl-use-plain-color-material.cpp"
#endif
