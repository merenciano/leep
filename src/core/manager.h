#ifndef __LEEP_CORE_MANAGER_H__
#define __LEEP_CORE_MANAGER_H__ 1

#include "core/window.h"
#include "core/input.h"
#include "core/memory.h"
#include "render/renderer.h"
#include "render/camera.h"
#include "tools/imgui-tools.h"

namespace leep 
{
    class Manager
    {
    public:
        static Manager& instance() { static Manager m; return m; }

        ~Manager() {}

        void init();

        Window&     window()   { return window_; }
        Renderer&   renderer() { return renderer_; }
        Camera&     camera()   { return camera_; }
        Input&      input()    { return input_; }
        Memory&     memory()   { return memory_; }
        ImguiTools& ui_tools() { return ui_tools_; }

    private:
        Window      window_;
        Renderer    renderer_;
        Camera      camera_;
        Input       input_;
        Memory      memory_;
        ImguiTools  ui_tools_;
    
    private:
        Manager() {}
        Manager(const Manager&) {}
        Manager& operator=(const Manager&) { return *this; }
    };
}

#endif // __LEEP_CORE_MANAGER_H__
