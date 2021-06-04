#include "swap-buffers.h"
#include "core/manager.h"
#include "core/window.h"

namespace leep
{
    SwapBuffers::SwapBuffers()
    {

    }

    SwapBuffers::~SwapBuffers()
    {

    }

    void SwapBuffers::executeCommand() const
    {
        GM.window().swap();
        GM.window().pollEvents();
    }
}
