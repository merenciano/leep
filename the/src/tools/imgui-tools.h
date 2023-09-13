#ifndef THE_TOOLS_IMGUI_TOOLS_H
#define THE_TOOLS_IMGUI_TOOLS_H

#include "core/definitions.h"

typedef struct {
	uint64_t duration_micro;
	float duration_ms;
	float init_time_ms;
	float logic_average_ms;
	float render_average_ms;
} THE_ToolsData;

extern THE_ToolsData tools_data;

void THE_UIToolsInit(void *raw_window);
void THE_UIToolsUpdate(void);
void THE_UIToolsRender(void);

// Without the average the number change so often
// that it's impossible to read it 
void THE_UIToolsCalcLogicAverage(float frame);

// This function adds 64 values and then div by 32 because
// each frame it takes 2 values that add together (render and swap times)
void THE_UIToolsCalcRenderAverage(float frame);
bool THE_UIToolsWantMouse(void);
bool THE_UIToolsWantKeyboard(void);
void THE_UIToolsSetCallback(void(*callbackfun)(void));

#endif
