#include "execution.h"
#include "core/manager.h"
#include "core/chrono.h"
#include "render/renderer.h"
#include "tools/imgui-tools.h"
#include "tools/lua-scripting.h"

//#define MTR_ENABLED
#include "minitrace.h"

static VoidFunc LogicF;
static VoidFunc CloseF;

void THE_Init(THE_Config *cnfg)
{
	mtr_init("the_trace.json");
	MTR_META_PROCESS_NAME("The_Profiling");
	MTR_META_THREAD_NAME("Main_Thread");
	MTR_BEGIN("THE", "Init");
	THE_Chrono init_timer;
	THE_ChronoStart(&init_timer);
	THE_InitManager(cnfg);
	THE_ScriptingInit();
	LogicF = cnfg->logic_func;
	CloseF = cnfg->close_func;

	cnfg->init_func();

	THE_ScriptingExecute("../assets/scripts/init.lua");
	THE_ChronoEnd(&init_timer);
	tools_data.init_time_ms = THE_ChronoDurationMS(&init_timer);
	THE_LOG("THE initialized in %f milliseconds.\n", tools_data.init_time_ms);
	MTR_END("THE", "Init");
}

void THE_Logic()
{
	MTR_META_THREAD_NAME("Logic_Thread");
	MTR_BEGIN("THE", "Logic");
	THE_Chrono logic_timer;
	THE_ChronoStart(&logic_timer);

	LogicF();

	THE_ChronoEnd(&logic_timer);
	THE_UIToolsCalcLogicAverage(THE_ChronoDurationMS(&logic_timer));
	MTR_END("THE", "Logic");
}

void THE_Render()
{
	MTR_BEGIN("THE", "Render");
	THE_Chrono render_timer;
	THE_ChronoStart(&render_timer);
	THE_RenderFrame();

	THE_UIToolsUpdate();
	THE_ChronoEnd(&render_timer);
	THE_UIToolsCalcRenderAverage(THE_ChronoDurationMS(&render_timer));
	MTR_END("THE", "Render");
}

void THE_ShowFrame()
{
	MTR_BEGIN("THE", "Swap_Buffers");
	THE_Chrono swap_timer;
	THE_ChronoStart(&swap_timer);
	THE_NextFrame();
	THE_ChronoEnd(&swap_timer);
	// Swap duration adds to render time
	THE_UIToolsCalcRenderAverage(THE_ChronoDurationMS(&swap_timer));
	MTR_END("THE", "Swap_Buffers");
}

void THE_Close()
{
	CloseF();
	mtr_flush();
	mtr_shutdown();
}
