// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_CREATE_BUFFER_COMMAND_H__
#define __LEEP_RENDER_CREATE_BUFFER_COMMAND_H__ 1

#include "render/buffer.h"
#include "render/display-list-command.h"

namespace leep
{
    class CreateBuffer : public DLComm
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

#endif // __LEEP_RENDER_CREATE_BUFFER_COMMAND_H__