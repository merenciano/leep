#include "plain-color.h"

#include "core/common-defs.h"

namespace leep
{
    PlainColor::PlainColor()
    {
        internal_id_ = ConstantValues::UNINITIALIZED_INTERNAL_ID;
    }

    PlainColor::~PlainColor()
    {

    }

    uint32_t PlainColor::internal_id() const
    {
        return internal_id_;
    }
}

#ifdef LEEP_OPENGL
    #include "render/materials/opengl/opengl-plain-color.cpp"
#endif

#ifdef LEEP_OPENGL_ES
    #include "render/materials/opengl-es/opengl-es-plain-color.cpp"
#endif