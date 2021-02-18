#ifndef __LEEP_TOOLS_IMGUI_TOOLS_H__
#define __LEEP_TOOLS_IMGUI_TOOLS_H__ 1

#include <stdint.h>
#include <string>

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

		void entityInspector();
		void componentInspector();
        void resourceInspector();

        // Without the average the number change so often
        // that it's impossible to read it 
        void calcLogicAverage(float frame) const;
        void calcRenderAverage(float frame) const;

        bool wantMouse() const;
        bool wantKeyboard() const;
		
	private:
		std::string selected_entity_;
		bool show_components_;
    };
}

#endif // __LEEP_TOOLS_IMGUI_TOOLS_H__