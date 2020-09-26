#include "render/commands/use-plain-color-material.h"

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
    #include "opengl/opengl-use-pbr-material.cpp"
#endif