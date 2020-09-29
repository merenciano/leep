#include "render/materials/plain-color.h"

#include "core/common-defs.h"

namespace motoret
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

#ifdef MOTORET_OPENGL
    #include "render/materials/opengl/opengl-plain-color.cpp"
#endif