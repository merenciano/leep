// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_COMMANDS_USE_FRAMEBUFFER_H__
#define __LEEP_RENDER_COMMANDS_USE_FRAMEBUFFER_H__ 1

#include "render/display-list-command.h"
#include "render/framebuffer.h"
 
namespace leep
{
    class UseFramebuffer : public DLComm
    {
    public:
        UseFramebuffer();
        UseFramebuffer(const UseFramebuffer&) = delete;
        UseFramebuffer(UseFramebuffer&&) = delete;
        ~UseFramebuffer();

        UseFramebuffer& set_framebuffer(const Framebuffer &fb);

        virtual void executeCommand() const override;

    private:
        Framebuffer fb_;
        bool default_;
    };
}

#endif // __LEEP_RENDER_COMMANDS_USE_FRAMEBUFFER_H__
