// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_COMMANDS_RENDER_OPTIONS_H__
#define __LEEP_RENDER_COMMANDS_RENDER_OPTIONS_H__

#define LEEP_BLEND_FUNC_BIT     1
#define LEEP_ENABLE_BLEND_BIT   1 << 1
#define LEEP_WRITE_DEPTH_BIT    1 << 2
#define LEEP_DEPTH_TEST_BIT     1 << 3
#define LEEP_CULL_FACE_BIT      1 << 4

#include "render/display-list-command.h"
#include <stdint.h>

namespace leep
{
    enum class BlendFunc
    {
        ONE,
        SRC_ALPHA,
        ONE_MINUS_SRC_ALPHA,
        ZERO,
    };

    enum class CullFace
    {
        DISABLED,
        FRONT,
        BACK,
        FRONT_AND_BACK,
    };

    class RenderOptions : public DLComm
    {
    private:
        struct Options
        {
            BlendFunc   sfactor_;
            BlendFunc   dfactor_;
            CullFace    cull_face_;
            bool        enable_depth_test_;
            bool        enable_write_depth_;
            bool        enable_blend_;
            uint32_t    dirty_mask_;
        };

    public:
        RenderOptions();       
        ~RenderOptions();       

        RenderOptions& enable_blend(bool enable);
        RenderOptions& set_blend_func(BlendFunc sfactor, BlendFunc dfactor);
        RenderOptions& set_cull_face(CullFace cull);
        RenderOptions& enable_write_depth(bool enable);
        RenderOptions& enable_depth_test(bool enable);

        virtual void executeCommand() const override;

    private:
        Options current_options_;
    };
}

#endif // __LEEP_RENDER_COMMANDS_RENDER_OPTIONS_H__