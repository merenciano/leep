#include "render/commands/render-options.h"

namespace leep
{
    RenderOptions::RenderOptions()
    {
        current_options_.sfactor_            = BlendFunc::ONE;
        current_options_.dfactor_            = BlendFunc::ZERO;
        current_options_.cull_face_          = CullFace::BACK;
        current_options_.enable_depth_test_  = true;
        current_options_.enable_write_depth_ = true;
        current_options_.enable_blend_       = true;
        current_options_.dirty_mask_         = 0;
    }

    RenderOptions::~RenderOptions()
    {

    }

    RenderOptions& RenderOptions::enable_blend(bool enable)
    {
        current_options_.dirty_mask_ |= LEEP_ENABLE_BLEND_BIT;
        current_options_.enable_blend_ = enable;
        return *this;
    }

    RenderOptions& RenderOptions::set_blend_func(
            BlendFunc sfactor,
            BlendFunc dfactor)
    {
        current_options_.dirty_mask_ |= LEEP_BLEND_FUNC_BIT;
        current_options_.sfactor_ = sfactor;
        current_options_.dfactor_ = dfactor;
        return *this;
    }

    RenderOptions& RenderOptions::set_cull_face(CullFace cull)
    {
        current_options_.dirty_mask_ |= LEEP_CULL_FACE_BIT;
        current_options_.cull_face_ = cull;
        return *this;
    }

    RenderOptions& RenderOptions::enable_write_depth(bool enable)
    {
        current_options_.dirty_mask_ |= LEEP_CULL_FACE_BIT;
        current_options_.enable_write_depth_ = enable;
        return *this;
    }

    RenderOptions& RenderOptions::enable_depth_test(bool enable)
    {
        current_options_.dirty_mask_ |= LEEP_DEPTH_TEST_BIT;
        current_options_.enable_depth_test_ = enable;
        return *this;
    }
}

#ifdef LEEP_OPENGL
    #include "render/commands/opengl/opengl-render-options.cpp"
#endif

#ifdef LEEP_OPENGL_ES
    #include "render/commands/opengl/opengl-render-options.cpp"
#endif