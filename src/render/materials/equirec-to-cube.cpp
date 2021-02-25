#include "equirec-to-cube.h"

namespace leep
{
    EquirecToCube::EquirecToCube()
    {
        internal_id_ = CommonDefs::UNINIT_INTERNAL_ID;
    }

    EquirecToCube::~EquirecToCube()
    {

    }

    uint32_t EquirecToCube::internal_id() const
    {
        return internal_id_;
    }
}

#ifdef LEEP_OPENGL
    #include "render/materials/opengl/opengl-equirec-to-cube.cpp"
#endif

#ifdef LEEP_OPENGL_ES
    #include "render/materials/opengl-es/opengl-es-equirec-to-cube.cpp"
#endif