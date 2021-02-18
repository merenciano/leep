#include "full-screen-image.h"

#include "core/common-defs.h"

namespace leep
{
    FullScreenImage::FullScreenImage()
    {
        internal_id_ = CommonDefs::UNINIT_INTERNAL_ID;
    }

    FullScreenImage::~FullScreenImage()
    {

    }

    uint32_t FullScreenImage::internal_id() const
    {
        return internal_id_;
    }
}

#ifdef LEEP_OPENGL
    #include "render/materials/opengl/opengl-full-screen-image.cpp"
#endif

#ifdef LEEP_OPENGL_ES
    #include "render/materials/opengl-es/opengl-es-full-screen-image.cpp"
#endif