#include "lut-gen.h"

#include "core/common-defs.h"

namespace leep {

LutGen::LutGen()
{
    internal_id_ = CommonDefs::UNINIT_INTERNAL_ID;
}

LutGen::~LutGen()
{

}

uint32_t LutGen::internal_id() const
{
    return internal_id_;
}

} // namespace leep

#ifdef LEEP_OPENGL
    #include "render/materials/opengl/opengl-lut-gen.cpp"
#endif

#ifdef LEEP_OPENGL_ES
    #include "render/materials/opengl-es/opengl-es-lut-gen.cpp"
#endif