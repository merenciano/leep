#include "prefilter-env.h"

namespace leep {

PrefilterEnv::PrefilterEnv()
{

}

PrefilterEnv::~PrefilterEnv()
{

}


uint32_t PrefilterEnv::internal_id() const
{
    return internal_id_;
}
}

#ifdef LEEP_OPENGL
    #include "render/materials/opengl/opengl-prefilter-env.cpp"
#endif

#ifdef LEEP_OPENGL_ES
    #include "render/materials/opengl-es/opengl-es-prefilter-env.cpp"
#endif
