#include "manager.h"
#include "core/common-defs.h"

#include "core/window.h"
#include "core/input.h"
#include "core/chrono.h"
#include "core/memory/memory.h"
#include "core/scene-graph.h"
#include "render/renderer.h"
#include "render/camera.h"
#include "tools/imgui-tools.h"
#include "tools/resource-map.h"
#include "tools/lua-scripting.h"

namespace leep
{
    Geometry Renderer::s_cube;
    Geometry Renderer::s_sphere;
    Geometry Renderer::s_quad;

    struct Manager::ManagerData
    {
        Window      window_;
        Renderer    renderer_;
        Camera      camera_;
        Input       input_;
        Memory      memory_;
        ImguiTools  ui_tools_;
        SceneGraph  scene_graph_;
        ResourceMap resource_map_;
        ToolsData   tools_data_;

        float       delta_time_;
        Chrono      frame_timer_;
    };

    void Manager::init()
    {
        LEEP_ASSERT(IsPow2(kEntitiesPerChunk),
            "This constant value must be power of 2");
        // Manager has the same life as the app, so I don't care of its deletion 
        data_ = new ManagerData();
        data_->window_.createWindow(1280, 720, false);
        data_->renderer_.init();
        data_->camera_.init();
        data_->delta_time_ = 0.16f;

        Renderer::s_cube.createCube();
		Renderer::s_sphere.createSphere(500, 500);
		Renderer::s_quad.createQuad();
    }

    float Manager::delta_time() const
    {
        return data_->delta_time_;
    }

    void Manager::nextFrame()
    {
        window().swap();
        window().pollEvents();
        renderer().submitFrame();
        data_->frame_timer_.end();
        data_->delta_time_ = data_->frame_timer_.duration() / 1000.0f;
        data_->frame_timer_.start();

        LuaScripting::SetGlobal("g_deltatime", data_->delta_time_);
    }

    void Manager::startFrameTimer()
    {
        data_->frame_timer_.start();
    }

    Window&      Manager::window()        { return data_->window_; }
    Renderer&    Manager::renderer()      { return data_->renderer_; }
    Camera&      Manager::camera()        { return data_->camera_; }
    Input&       Manager::input()         { return data_->input_; }
    Memory&      Manager::memory()        { return data_->memory_; }
    ImguiTools&  Manager::ui_tools()      { return data_->ui_tools_; }
    SceneGraph&  Manager::scene_graph()   { return data_->scene_graph_; }
    ResourceMap& Manager::resource_map()  { return data_->resource_map_; }
    ToolsData&   Manager::tools_data()    { return data_->tools_data_; }
}