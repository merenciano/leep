// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_COMMANDS_CREATE_FRAMEBUFFER_H__
#define __LEEP_RENDER_COMMANDS_CREATE_FRAMEBUFFER_H__

#include "render/framebuffer.h"
#include "render/display-list-command.h"

namespace leep
{
    class CreateFramebuffer : public DLComm
    {
    public:
        CreateFramebuffer() = default;
        CreateFramebuffer(const CreateFramebuffer &other) = delete;
        CreateFramebuffer(CreateFramebuffer &&other) = delete;
        ~CreateFramebuffer() = default;

        CreateFramebuffer& set_framebuffer(const Framebuffer &framebuffer);
        virtual void executeCommand() const override;

    private:
        Framebuffer fb_;
    };
}

#endif // __LEEP_RENDER_COMMANDS_CREATE_FRAMEBUFFER_H__