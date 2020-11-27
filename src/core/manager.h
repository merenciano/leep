#ifndef __MOTORET_CORE_MANAGER_H__
#define __MOTORET_CORE_MANAGER_H__ 1

#include "core/window.h"
#include "ecs/internal-entity.h"
#include "render/renderer.h"

namespace motoret
{
    class Manager
    {
    public:
        static Manager& instance() { static Manager m; return m; }

        ~Manager() {}

        Window&   window() { return window_; }
        Renderer& renderer() { return renderer_; }

        std::vector<InternalEntity> entities_;
        std::unordered_map<std::string, int32_t> entity_map_;

    private:
        Window   window_;
        Renderer renderer_;
    
    private:
        Manager() {}
        Manager(const Manager&) {}
        Manager& operator=(const Manager&) { return *this; }
    };
}

#endif // __MOTORET_CORE_MANAGER_H__
