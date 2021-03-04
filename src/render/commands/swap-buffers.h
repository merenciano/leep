// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_COMMANDS_SWAP_BUFFERS_H__
#define __LEEP_RENDER_COMMANDS_SWAP_BUFFERS_H__ 1

#include "render/display-list-command.h"

namespace leep
{
    class SwapBuffers : public DLComm
    {
    public:
        SwapBuffers();
        ~SwapBuffers();

        virtual void executeCommand() const override;
    };

}

#endif // __LEEP_RENDER_COMMANDS_SWAP_BUFFERS_H__
