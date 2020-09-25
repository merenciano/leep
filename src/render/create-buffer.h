#ifndef __MOTORET_RENDER_CREATE_BUFFER_COMMAND_H__
#define __MOTORET_RENDER_CREATE_BUFFER_COMMAND_H__ 1

#include "render/buffer.h"
#include "render/display-list-command.h"

namespace motoret
{
    class CreateBuffer : public DisplayListCommand
    {
    public:
        CreateBuffer();
        CreateBuffer(const CreateBuffer &other) = delete;
        CreateBuffer(CreateBuffer &&other) = delete;
        ~CreateBuffer();

        CreateBuffer& set_buffer(const Buffer &buffer);

        virtual void executeCommand() const override;

    private:
        Buffer buffer_;
    };
}

#endif // __MOTORET_RENDER_CREATE_BUFFER_COMMAND_H__