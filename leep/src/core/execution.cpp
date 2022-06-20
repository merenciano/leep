#include "execution.h"
#include "core/common-defs.h"
#include "core/chrono.h"
#include "render/Crenderer.h"
#include "tools/imgui-tools.h"
#include "tools/lua-scripting.h"

//#define MTR_ENABLED
#include "minitrace.h"

void leep::Init(const LeepConfig &cnfg)
{
    mtr_init("leep_trace.json");
    MTR_META_PROCESS_NAME("Leep_Profiling");
    MTR_META_THREAD_NAME("Main_Thread");
    MTR_BEGIN("LEEP", "Init");
    leep::Chrono init_timer;
    init_timer.start();
    leep::GM.init(cnfg);
    leep::LuaScripting::Init();

    leep::GameInit();

    leep::LuaScripting::ExecuteScript("../assets/scripts/init.lua");
    init_timer.end();
    leep::GM.tools_data().init_time_ms_ = init_timer.duration();
    //LEEP_CORE_INFO("Leep initialized in {0} milliseconds.", init_timer.duration());
    MTR_END("LEEP", "Init");
}

void leep::Logic()
{
    MTR_META_THREAD_NAME("Logic_Thread");
    MTR_BEGIN("LEEP", "Logic");
    leep::Chrono logic_timer;
    logic_timer.start();

    leep::GameLogic();

    logic_timer.end();
    leep::GM.ui_tools().calcLogicAverage(logic_timer.duration());
    MTR_END("LEEP", "Logic");
}

void leep::RenderFrame()
{
    MTR_BEGIN("LEEP", "Render");
    leep::Chrono render_timer;
    render_timer.start();
    THE_RenderFrame();

    leep::GM.ui_tools().update();
    render_timer.end();
    leep::GM.ui_tools().calcRenderAverage(render_timer.duration());
    MTR_END("LEEP", "Render");
}

void leep::ShowFrame()
{
    MTR_BEGIN("LEEP", "Swap_Buffers");
    leep::Chrono swap_timer;
    swap_timer.start();
    leep::GM.nextFrame();
    swap_timer.end();
    // Swap duration adds to render time
    leep::GM.ui_tools().calcRenderAverage(swap_timer.duration());
    MTR_END("LEEP", "Swap_Buffers");
}

void leep::Close()
{
    leep::GameClose();
    mtr_flush();
    mtr_shutdown();
}