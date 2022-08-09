#ifndef THE_TOOLS_IMGUI_TOOLS_H
#define THE_TOOLS_IMGUI_TOOLS_H

#include "core/Cdefinitions.h"

typedef struct THE_ToolsData {
	u64 duration_micro;
	float duration_ms;
	float init_time_ms;
	float logic_average_ms;
	float render_average_ms;
} THE_ToolsData;

void THE_UIToolsInit(void *raw_window);
void THE_UIToolsUpdate();
void THE_UIToolsRender();

//void THE_UIToolsInfoWindow(bool *show);
//void THE_UIToolsLuaCommands();
//void THE_UIToolsEntityInspector();
//void THE_UIToolsComponentInspector();
//void THE_UIToolsResourceInspector();
//void THE_UIToolsMemoryUsage();

// Without the average the number change so often
// that it's impossible to read it 
void THE_UIToolsCalcLogicAverage(float frame);

// This function adds 64 values and then div by 32 because
// each frame it takes 2 values that add together (render and swap times)
void THE_UIToolsCalcRenderAverage(float frame);
bool THE_UIToolsWantMouse();
bool THE_UIToolsWantKeyboard();
void THE_UIToolsSetCallback(void(*callbackfun)(void));
	
#if 0
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
#endif

#endif
