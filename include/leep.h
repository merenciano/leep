// CORE
#include "../src/core/common-defs.h"
#include "../src/core/manager.h"
#include "../src/core/window.h"
#include "../src/core/logger.h"
#include "../src/core/chrono.h"
#include "../src/core/input.h"
#include "../src/core/scene.h"
#include "../src/core/logic-thread.h"
#include "../src/core/memory/memory.h"

// ECS
#include "../src/ecs/entity.h"
#include "../src/ecs/component.h"
#include "../src/ecs/system.h"
#include "../src/ecs/components/drawable.h"
#include "../src/ecs/components/ltransform.h"
#include "../src/ecs/components/gtransform.h"
#include "../src/ecs/components/fall-speed.h"
#include "../src/ecs/components/infinite-falling-limits.h"
#include "../src/ecs/systems/render.h"
#include "../src/ecs/systems/camera-movement.h"
#include "../src/ecs/systems/fall.h"
#include "../src/ecs/systems/infinite-falling.h"
#include "../src/ecs/systems/update-transform.h"
#include "../src/ecs/systems/update-scene-graph.h"
#include "../src/ecs/systems/delete-released.h"

// RENDER
#include "../src/render/renderer.h"
#include "../src/render/camera.h"
#include "../src/render/texture.h"
#include "../src/render/display-list.h"
#include "../src/render/display-list-command.h"
#include "../src/render/commands/clear.h"
#include "../src/render/commands/create-buffer.h"
#include "../src/render/commands/use-pbr-material.h"
#include "../src/render/commands/use-skybox-material.h"
#include "../src/render/commands/use-framebuffer.h"
#include "../src/render/commands/draw.h"
#include "../src/render/commands/draw-skybox.h"
#include "../src/render/commands/create-texture.h"
#include "../src/render/commands/render-options.h"
#include "../src/render/commands/equirectangular-to-cubemap.h"

// TOOLS
#include "../src/tools/imgui-tools.h"
#include "../src/tools/lua-scripting.h"
#include "../src/tools/resource-map.h"

//#define MTR_ENABLED
#include "minitrace.h"

#define LEEP_SINGLE_THREAD 0

namespace leep 
{
    void Init();
    void Logic();
}

void LeepInit()
{
    MTR_BEGIN("LEEP", "Init");
    leep::Chrono init_timer;
    init_timer.start();
    leep::Logger::Init();
    leep::GM.init();
    leep::LuaScripting::Init();

    leep::Init();

    leep::LuaScripting::ExecuteScript("../assets/scripts/init.lua");
    init_timer.end();
    leep::GM.tools_data().init_time_ms_ = init_timer.duration();
    //LEEP_CORE_INFO("Leep initialized in {0} milliseconds.", init_timer.duration());
    MTR_END("LEEP", "Init");
}

void LeepLogic()
{
    MTR_META_THREAD_NAME("Logic_Thread");
    MTR_BEGIN("LEEP", "Logic");
    leep::Chrono logic_timer;
    logic_timer.start();

    leep::Logic();

    logic_timer.end();
    leep::GM.ui_tools().calcLogicAverage(logic_timer.duration());
    MTR_END("LEEP", "Logic");
}

void LeepRender()
{
    MTR_BEGIN("LEEP", "Render");
    leep::Chrono render_timer;
    render_timer.start();
    leep::GM.renderer().renderFrame();

    leep::GM.ui_tools().update();
    render_timer.end();
    leep::GM.ui_tools().calcRenderAverage(render_timer.duration());
    MTR_END("LEEP", "Render");
}

int main(int argc, char **argv)
{
    mtr_init("leep_trace.json");
    MTR_META_PROCESS_NAME("Leep_Profiling");
    MTR_META_THREAD_NAME("Main_Thread");
    LeepInit();
    leep::LogicThread lt(LeepLogic);
    leep::GM.startFrameTimer();
    while (!leep::GM.window().windowShouldClose())
    {
#if LEEP_SINGLE_THREAD == 0
        leep::LuaScripting::ExecuteScript("../assets/scripts/update.lua");

        lt.run();
        LeepRender();
        while (lt.running())
        {
            //LEEP_CORE_WARNING("Logic loop slower than render");
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
#elif LEEP_SINGLE_THREAD == 1
        LeepLogic();
        LeepRender();
#endif
        MTR_BEGIN("LEEP", "Swap_Buffers");
        leep::Chrono swap_timer;
        swap_timer.start();
        leep::GM.nextFrame();
        swap_timer.end();
        // Swap duration adds to render time
        leep::GM.ui_tools().calcRenderAverage(swap_timer.duration());
        MTR_END("LEEP", "Swap_Buffers");
    }
    mtr_flush();
    mtr_shutdown();

    return 0;
}
