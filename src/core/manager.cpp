#include "manager.h"
#include "core/common-defs.h"

namespace leep
{
    void Manager::init()
    {
        LEEP_ASSERT(IsPow2(kEntitiesPerChunk), "This constant value must be power of 2");
        window_.createWindow(1280, 720, true);
        renderer_.init();
        camera_.init();
    }
}