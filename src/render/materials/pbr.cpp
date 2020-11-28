#include "pbr.h"

namespace leep
{
    Pbr::Pbr()
    {

    }

    Pbr::~Pbr()
    {

    }


    uint32_t Pbr::internal_id() const
    {
        return internal_id_;
    }
}

#ifdef LEEP_OPENGL
    #include "render/materials/opengl/opengl-pbr.cpp"
#endif