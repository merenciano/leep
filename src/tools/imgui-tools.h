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
        void update();
        void render();

        bool wantMouse() const;
        bool wantKeyboard() const;
    };
}

#endif // __LEEP_TOOLS_IMGUI_TOOLS_H__