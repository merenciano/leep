/* Lucas Merenciano Martinez  <lucasmermar@gmail.com> */

#ifndef __LEEP_CORE_CONFIG_H__
#define __LEEP_CORE_CONFIG_H__

#include "core/common-defs.h"

namespace leep
{
    const uint64_t kTotalMemSize = MEGABYTES((uint64_t)1224);
    const int32_t kWindowWidth = 1280;
    const int32_t kWindowHeight = 800;

    class Config
    {
        bool vsync;            
    };

    class GameData
    {
        float delta_time_;
    };
}

#endif // __LEEP_CORE_CONFIG_H__
