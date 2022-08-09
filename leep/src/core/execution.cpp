#include "execution.h"
#include "core/common-defs.h"
#include "core/chrono.h"
#include "render/Crenderer.h"
#include "tools/imgui-tools.h"
#include "tools/lua-scripting.h"

//#define MTR_ENABLED
#include "minitrace.h"

void leep::Init(const THE_Config &cnfg)
{
    mtr_init("leep_trace.json");
    MTR_META_PROCESS_NAME("Leep_Profiling");
    MTR_META_THREAD_NAME("Main_Thread");
    MTR_BEGIN("LEEP", "Init");
    THE_Chrono init_timer;
    THE_ChronoStart(&init_timer);
    leep::GM.init(cnfg);
    leep::LuaScripting::Init();

    leep::GameInit();

    leep::LuaScripting::ExecuteScript("../assets/scripts/init.lua");
    THE_ChronoEnd(&init_timer);
    leep::GM.tools_data().init_time_ms_ = THE_ChronoDurationMS(&init_timer);
    THE_LOG("Leep initialized in %f milliseconds.\n", leep::GM.tools_data().init_time_ms_);
    MTR_END("LEEP", "Init");
}

void leep::Logic()
{
    MTR_META_THREAD_NAME("Logic_Thread");
    MTR_BEGIN("LEEP", "Logic");
    THE_Chrono logic_timer;
    THE_ChronoStart(&logic_timer);

    leep::GameLogic();

    THE_ChronoEnd(&logic_timer);
    leep::GM.ui_tools().calcLogicAverage(THE_ChronoDurationMS(&logic_timer));
    MTR_END("LEEP", "Logic");
}

void leep::RenderFrame()
{
    MTR_BEGIN("LEEP", "Render");
    THE_Chrono render_timer;
    THE_ChronoStart(&render_timer);
    THE_RenderFrame();

    leep::GM.ui_tools().update();
    THE_ChronoEnd(&render_timer);
    leep::GM.ui_tools().calcRenderAverage(THE_ChronoDurationMS(&render_timer));
    MTR_END("LEEP", "Render");
}

void leep::ShowFrame()
{
    MTR_BEGIN("LEEP", "Swap_Buffers");
    THE_Chrono swap_timer;
    THE_ChronoStart(&swap_timer);
    leep::GM.nextFrame();
    THE_ChronoEnd(&swap_timer);
    // Swap duration adds to render time
    leep::GM.ui_tools().calcRenderAverage(THE_ChronoDurationMS(&swap_timer));
    MTR_END("LEEP", "Swap_Buffers");
}

void leep::Close()
{
    leep::GameClose();
    mtr_flush();
    mtr_shutdown();
}
