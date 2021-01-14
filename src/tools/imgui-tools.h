#ifndef __LEEP_TOOLS_IMGUI_TOOLS_H__
#define __LEEP_TOOLS_IMGUI_TOOLS_H__ 1

#include <stdint.h>

namespace leep
{
    struct ToolsData
    {
        float duration_ms_;
        uint64_t duration_micro_;
    };

    class ImguiTools
    {
    public:
        ImguiTools();
        ~ImguiTools();
        void init(void *raw_window);
        void update(float delta_time, int32_t width, int32_t height);
    };
}

#endif // __LEEP_TOOLS_IMGUI_TOOLS_H__