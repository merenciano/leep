#include "manager.h"
#include "core/common-defs.h"

#include "core/io.h"
#include "core/chrono.h"
#include "core/memory/memory.h"
#include "core/scene.h"
#include "render/Crenderer.h"
#include "tools/imgui-tools.h"
#include "tools/lua-scripting.h"
#include "tools/resource-map.h"
#include "core/hmap.h"

#include "render/Cinternalresources.h"
#include "render/Crendercommands.h"

namespace leep
{
    struct Manager::ManagerData
    {
        ImguiTools  ui_tools_;
        Scene       scene_;
        ::THE_ResourceMap *resource_map_;
        ToolsData   tools_data_;

        float       delta_time_;
        Chrono      frame_timer_;
    };

    void Manager::init(const THE_Config &cnfg)
    {
        THE_ASSERT(IsPow2(kEntitiesPerChunk), "This constant value must be power of 2");

        size_t total_mem = kMaxBuffers * sizeof(THE_InternalBuffer) + kMaxTextures * sizeof(THE_InternalTexture);
        total_mem += THE_MT_MAX * sizeof(THE_InternalMaterial);
        total_mem += cnfg.render_queue_capacity * 2 * sizeof(void*); // 2 because current and next
        total_mem += cnfg.render_queue_capacity * 2 * sizeof(THE_CommandData);
        total_mem += cnfg.alloc_capacity;
        memory_.init(total_mem);
        data_ = memory_.generalAllocT<ManagerData>(1);
	THE_IOInit(kWindowTitle, cnfg.window_width, cnfg.window_height, cnfg.vsync);
	data_->resource_map_ = (THE_ResourceMap*)memory_.generalAlloc(sizeof(THE_ResourceMap));
	data_->resource_map_->meshes = THE_HMapCreate(8, sizeof(THE_Mesh));
	data_->resource_map_->textures = THE_HMapCreate(64, sizeof(THE_Texture));
	THE_InitRender();
        data_->delta_time_ = 0.16f;
    }

    float Manager::delta_time() const
    {
        return data_->delta_time_;
    }

    void Manager::nextFrame()
    {
	THE_WindowSwapBuffers();
	THE_IOPollEvents();
	THE_SubmitFrame();

        data_->frame_timer_.end();
        data_->delta_time_ = data_->frame_timer_.duration() / 1000.0f;
        data_->frame_timer_.start();

        LuaScripting::SetGlobal("g_deltatime", data_->delta_time_);
    }

    void Manager::startFrameTimer()
    {
        data_->frame_timer_.start();
    }

    Memory&      Manager::memory()        { return memory_; }
    ImguiTools&  Manager::ui_tools()      { return data_->ui_tools_; }
    Scene&       Manager::scene()         { return data_->scene_; }
    ::THE_ResourceMap *Manager::resource_map()  { return data_->resource_map_; }
    ToolsData&   Manager::tools_data()    { return data_->tools_data_; }
}
