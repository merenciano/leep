#include "manager.h"
#include "core/common-defs.h"

namespace leep
{
    void Manager::init()
    {
        Logger::init();
        window_.createWindow(1280, 720, true);
        renderer_.init();
        camera_.init();
    }
}