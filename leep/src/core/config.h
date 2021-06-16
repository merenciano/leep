/* Lucas Merenciano Martinez  <lucasmermar@gmail.com> */

#ifndef __LEEP_CORE_CONFIG_H__
#define __LEEP_CORE_CONFIG_H__

#include "core/common-defs.h"

namespace leep
{
    static const char *kWindowTitle = "Leep";
    const int32_t kMaxTextures = 63;
    const int32_t kMaxBuffers = 128;

    // TODO: Change to Config since is inside leep namespace
    struct LeepConfig
    {
        // Memory chunk size for the buddy allocator (in bytes)
        size_t alloc_capacity = GIGABYTES((uint64_t)1);
        // Maximum renderable entities in the scene
        // This is for engine allocation purposes
        int32_t render_queue_capacity = 12000;
        int32_t max_geometries = 128;
        int32_t window_width = 1280;
        int32_t window_height = 720;
        bool vsync = true;
    };

    class GameData
    {
        float delta_time_;
    };
}

#endif // __LEEP_CORE_CONFIG_H__
