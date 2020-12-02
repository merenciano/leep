// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_COMMANDS_RENDER_OPTIONS_H__
#define __LEEP_RENDER_COMMANDS_RENDER_OPTIONS_H__ 1

#include "render/display-list-command.h"
#include <stdint.h>

namespace leep
{
    enum class BlendFunc
    {
        NONE,
        ONE,
        SRC_ALPHA,
        ONE_MINUS_SRC_ALPHA,
        ZERO
    };

    enum class CullFace
    {
        NONE,
        FRONT,
        BACK,
        FRONT_AND_BACK
    };

    struct Options
    {
        BlendFunc   sfactor = BlendFunc::ONE;
        BlendFunc   dfactor = BlendFunc::ZERO;
        CullFace    cull_face = CullFace::BACK;
        bool        read = true;
        bool        write = true;  
    };

    class RenderOptions : public DisplayListCommand
    {
    public:
        RenderOptions();       
        ~RenderOptions();       

        RenderOptions& set_blend(BlendFunc sfactor, BlendFunc dfactor);
        RenderOptions& set_cull_face(CullFace cull);
        RenderOptions& set_depth(bool read, bool write);

        virtual void executeCommand() const override;

    private:
        static Options s_current_options;
    };
}

#endif // __LEEP_RENDER_COMMANDS_RENDER_OPTIONS_H__