#include "manager.h"
#include "core/common-defs.h"

#include "core/window.h"
#include "core/input.h"
#include "core/chrono.h"
#include "core/memory/memory.h"
#include "core/scene.h"
#include "render/renderer.h"
#include "render/commands/draw.h" // This is needed for sizeof(Draw) /* Change that and remove dependency? */
#include "render/camera.h"
#include "tools/imgui-tools.h"
#include "tools/resource-map.h"
#include "tools/lua-scripting.h"

namespace leep
{
    Geometry Renderer::s_cube;
    Geometry Renderer::s_voxel;
    Geometry Renderer::s_sphere;
    Geometry Renderer::s_quad;

    struct Manager::ManagerData
    {
        Window      window_;
        Renderer    renderer_;
        Camera      camera_;
        Input       input_;
        ImguiTools  ui_tools_;
        Scene       scene_;
        ResourceMap resource_map_;
        ToolsData   tools_data_;

        float       delta_time_;
        Chrono      frame_timer_;
    };

    void Manager::init(const LeepConfig &cnfg)
    {
        LEEP_ASSERT(IsPow2(kEntitiesPerChunk),
            "This constant value must be power of 2");

        size_t total_mem = kMaxBuffers * sizeof(InternalBuffer) + kMaxTextures * sizeof(InternalTexture);
        total_mem += MaterialType::MT_MAX * sizeof(InternalMaterial);
        total_mem += cnfg.render_queue_capacity * 2 * sizeof(void*); // 2 because current and next
        total_mem += cnfg.render_queue_capacity * 2 * sizeof(Draw);
        total_mem += cnfg.alloc_capacity;
        memory_.init(total_mem);
        data_ = memory_.generalAllocT<ManagerData>(1);
        data_->window_.createWindow(kWindowTitle, cnfg.window_width, cnfg.window_height, cnfg.vsync);
        data_->renderer_.init(cnfg.render_queue_capacity);
        data_->camera_.init(70.0f, 300.0f);
        data_->delta_time_ = 0.16f;

        Renderer::s_cube.createCube();
        Renderer::s_voxel.createVoxel();
		Renderer::s_sphere.createSphere(32, 32);
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
    Memory&      Manager::memory()        { return memory_; }
    ImguiTools&  Manager::ui_tools()      { return data_->ui_tools_; }
    Scene&       Manager::scene()         { return data_->scene_; }
    ResourceMap& Manager::resource_map()  { return data_->resource_map_; }
    ToolsData&   Manager::tools_data()    { return data_->tools_data_; }
}
