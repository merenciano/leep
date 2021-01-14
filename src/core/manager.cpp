#include "manager.h"
#include "core/common-defs.h"

#include "core/window.h"
#include "core/input.h"
#include "core/memory/memory.h"
#include "core/scene-graph.h"
#include "render/renderer.h"
#include "render/camera.h"
#include "tools/imgui-tools.h"
#include "tools/resource-map.h"

namespace leep
{
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
    };

    void Manager::init()
    {
        LEEP_ASSERT(IsPow2(kEntitiesPerChunk), "This constant value must be power of 2");
        data_ = new ManagerData();
        data_->window_.createWindow(1280, 720, true);
        data_->renderer_.init();
        data_->camera_.init();
    }

    Window&      Manager::window()        { return data_->window_; }
    Renderer&    Manager::renderer()      { return data_->renderer_; }
    Camera&      Manager::camera()        { return data_->camera_; }
    Input&       Manager::input()         { return data_->input_; }
    Memory&      Manager::memory()        { return data_->memory_; }
    ImguiTools&  Manager::ui_tools()      { return data_->ui_tools_; }
    SceneGraph&  Manager::scene_graph()   { return data_->scene_graph_; }
    ResourceMap& Manager::resource_map()  { return data_->resource_map_; }
}