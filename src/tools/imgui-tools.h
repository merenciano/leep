#ifndef __LEEP_TOOLS_IMGUI_TOOLS_H__
#define __LEEP_TOOLS_IMGUI_TOOLS_H__ 1

#include "core/string.h"
#include <stdint.h>

namespace leep
{
    struct ToolsData
    {
        float duration_ms_;
        uint64_t duration_micro_;
        float init_time_ms_;
        float logic_average_ms_;
        float render_average_ms_;
    };

    class ImguiTools
    {
    public:
        ImguiTools();
        ~ImguiTools();
        void init(void *raw_window);
        void update();
        void render();

        void infoWindow(bool *show);
        void luaCommands();
		void entityInspector();
		void componentInspector();
        void resourceInspector();
        void memoryUsage();

        // Without the average the number change so often
        // that it's impossible to read it 
        void calcLogicAverage(float frame) const;
        void calcRenderAverage(float frame) const;

        bool wantMouse() const;
        bool wantKeyboard() const;

        void set_callback(void(*callbacfun)(void));
		
	private:
        String selected_entity_;
		bool show_components_;
        bool show_lua_commands_;
        bool show_entity_inspector_;
        bool show_component_inspector_;
        bool show_resource_inspector_;
        bool show_memory_usage_;
        void (*callback_)(void);
    };
}

#endif // __LEEP_TOOLS_IMGUI_TOOLS_H__
