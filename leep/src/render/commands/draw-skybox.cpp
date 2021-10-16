#include "draw-skybox.h"
#include "core/manager.h"
#include "render/camera.h"

namespace leep
{
    DrawSkybox::DrawSkybox()
    {
        glm::mat4 view = GM.camera().static_view_projection();
        material_.create(MT_SKYBOX, (float*)&view, 16, nullptr, 0, 0);
    }

    DrawSkybox::~DrawSkybox()
    {

    }

    DrawSkybox& DrawSkybox::set_cubemap(const Texture &cubemap)
    {
        glm::mat4 view = GM.camera().static_view_projection();
        material_.create(MT_SKYBOX, (float*)&view, 16, (Texture*)&cubemap, 1, 0);
        return *this;
    }
}

#ifdef LEEP_OPENGL
    #include "render/commands/opengl/opengl-draw-skybox.cpp"
#endif

#ifdef LEEP_OPENGL_ES
    #include "render/commands/opengl/opengl-draw-skybox.cpp"
#endif
