#include "skybox.h"

namespace leep
{
    Skybox::Skybox()
    {

    }

    Skybox::~Skybox()
    {

    }


    uint32_t Skybox::internal_id() const
    {
        return internal_id_;
    }
}

#ifdef LEEP_OPENGL
    #include "render/materials/opengl/opengl-skybox.cpp"
#endif

#ifdef LEEP_OPENGL_ES
    #include "render/materials/opengl-es/opengl-es-skybox.cpp"
#endif