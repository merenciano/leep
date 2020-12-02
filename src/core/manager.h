#ifndef __LEEP_CORE_MANAGER_H__
#define __LEEP_CORE_MANAGER_H__ 1

#include "core/window.h"
#include "core/input.h"
#include "ecs/internal-entity.h"
#include "render/renderer.h"
#include "render/camera.h"

namespace leep 
{
    class Manager
    {
    public:
        static Manager& instance() { static Manager m; return m; }

        ~Manager() {}

        void init();

        Window&   window()   { return window_; }
        Renderer& renderer() { return renderer_; }
        Camera&   camera()   { return camera_; }
        Input&    input()    { return input_; }

        std::vector<InternalEntity> entities_;
        std::unordered_map<std::string, int32_t> entity_map_;

    private:
        Window   window_;
        Renderer renderer_;
        Camera   camera_;
        Input    input_;
    
    private:
        Manager() {}
        Manager(const Manager&) {}
        Manager& operator=(const Manager&) { return *this; }
    };
}

#endif // __LEEP_CORE_MANAGER_H__
