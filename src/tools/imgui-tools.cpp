#include "imgui-tools.h"
#include "core/manager.h"
#include "core/memory/memory.h"
#include "render/renderer.h"
#include "ecs/entity.h"
#include "ecs/components/ltransform.h"
#include "ecs/components/drawable.h"
#include "tools/lua-scripting.h"
#include "tools/resource-map.h"

#include <vector>
#include <deque>

#include <glm/gtc/type_ptr.hpp>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_opengl3.h> // TODO(Lucas): implement myself
#include <ImGui/imgui_impl_glfw.h>
#include <GLFW/glfw3.h> // remove this when implementing input

namespace leep
{
    static void BasicAppInfo();
    static void LuaCommands();

    ImguiTools::ImguiTools()
    {
		selected_entity_ = "";
        show_components_ = false;
    }

    ImguiTools::~ImguiTools()
    {

    }

    void ImguiTools::init(void *raw_window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)raw_window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    void ImguiTools::update()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        static bool show = true;
        if (show)
            ImGui::ShowDemoWindow(&show);

        BasicAppInfo();
        LuaCommands();
        entityInspector();
		componentInspector();
        resourceInspector();
        memoryInspector();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImguiTools::render()
    {

    }

    void ImguiTools::calcLogicAverage(float frame) const
    {
        static int32_t i = 0;
        static float previous[32];
        previous[i++] = frame;
        if (i == 32)
        {
            float avg = 0.0f;
            i = 0;
            for (int j = 0; j < 32; ++j)
            {
                avg += previous[j];
            }
            GM.tools_data().logic_average_ms_ = avg / 32.0f;
        }
    }

    void ImguiTools::calcRenderAverage(float frame) const
    {
        static int32_t i = 0;
        static float previous[32];
        previous[i++] = frame;
        if (i == 32)
        {
            float avg = 0.0f;
            i = 0;
            for (int j = 0; j < 32; ++j)
            {
                avg += previous[j];
            }
            GM.tools_data().render_average_ms_ = avg / 32.0f;
        }
    }

    bool ImguiTools::wantKeyboard() const
    {
        ImGuiIO &io = ImGui::GetIO();
        return io.WantCaptureKeyboard;
    }

    bool ImguiTools::wantMouse() const
    {
        ImGuiIO &io = ImGui::GetIO();
        return io.WantCaptureMouse;
    }

    static void BasicAppInfo()
    {
        static bool show = true;
        if (!ImGui::Begin("Basic APP Info", &show, 0))
        {
            // Early out if the window is collapsed, as an optimization.
            ImGui::End();
            return;
        }
        ImGui::Text("Frame time: %f", GM.delta_time() * 1000.0f);
        ImGui::Text("FPS: %f", 1.0f / GM.delta_time());
        ImGui::Text("Init time (ms): %f", GM.tools_data().init_time_ms_);
        ImGui::Text("Logic average (ms): %f", GM.tools_data().logic_average_ms_);
        ImGui::Text("Render average (ms): %f", GM.tools_data().render_average_ms_);
        ImGui::End();
    }

    static void LuaCommands()
    {
        static bool show = true;
        if (!ImGui::Begin("Insert Lua Commands", &show, 0))
        {
            // Early out if the window is collapsed, as an optimization.
            ImGui::End();
            return;
        }
        static char buffer[64];
        ImGui::InputText("Command input", buffer, IM_ARRAYSIZE(buffer));
        if (ImGui::Button("Execute"))
        {
            LuaScripting::ExecuteCommand(std::string(buffer));
        }
        ImGui::End();
    }

    void ImguiTools::entityInspector()
    {
        static bool show = true;
        if (!ImGui::Begin("Entity inspector", &show, 0))
        {
            // Early out if the window is collapsed, as an optimization.
            ImGui::End();
            return;
        }

        auto it = GM.memory().entities_.begin();
        while(it != GM.memory().entities_.end())
        {
            std::string header_name;
            switch(it->first)
            {
                default:
                    LEEP_ASSERT(false, "EntityInspector: EntityType not implemented")
                    break;

                case EntityType::NONE:
                    header_name = "";
                    break;

                case EntityType::RENDERABLE:
                    header_name = "Renderable";
                    break;

                case EntityType::FALLING_CUBE:
                    header_name = "InfiniteFalling";
                    break;
            }
            if (ImGui::CollapsingHeader(header_name.c_str()))
            {
                for (uint32_t i = 0; i < it->second.blocks_.size(); ++i)
                {
                    std::string tree_name = "Chunk " + std::to_string(i);
                    if (ImGui::TreeNode(tree_name.c_str()))
                    {
                        static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
                        ImVec2 size = ImVec2(-FLT_MIN, ImGui::GetTextLineHeightWithSpacing() * 12);
                        if (ImGui::BeginTable("##table1", 2, flags, size))
                        {
                            ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
                            ImGui::TableSetupColumn("Index", ImGuiTableColumnFlags_None);
                            ImGui::TableSetupColumn("Entity name", ImGuiTableColumnFlags_None);
                            ImGui::TableHeadersRow();

                            for (int32_t j = 0; j < it->second.blocks_[i]->last_; ++j)
                            {
                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::Text("%d", j);
                                ImGui::TableSetColumnIndex(1);
                                //ImGui::Text("%s", Entity(j, it->first).name().c_str());
                                if (ImGui::Button(Entity(j, it->first).name().c_str()))
                                {
                                    show_components_  = true;
                                    selected_entity_ = Entity(j, it->first).name();
                                }
                            }
                            ImGui::EndTable();
                        }
                        ImGui::TreePop();
                    }
                }
            }
            ++it;
        }
        ImGui::End();
    }

	void ImguiTools::componentInspector()
	{
		char window_title[64];
        if (selected_entity_ == "")
        {
            return;
        }

        snprintf(window_title, 64, "Components of %s", selected_entity_.c_str());
		if (!ImGui::Begin(window_title, &show_components_, 0))
		{
			// Early out if the window is collapsed, as an optimization.
			ImGui::End();
			return;
		}
        // Generating the entity from the name because the Entity type
        // can lose its reference if any entity is removed because
        // when that happens the last entity of the container is relocated
        // in order to fill the gap, so that last entity could be the one
        // stored in the entity variable and Entity::index_ would cause
        // a crash in the best case
        Entity e = Entity::GetEntity(selected_entity_);
        if (e.hasComponent<LTransform>())
        {

        }
        if (e.hasComponent<Drawable>())
        {
            Drawable& dw = e.getComponent<Drawable>();
            if (dw.material_.type() == MaterialType::MT_PBR)
            {
                PbrData data = dw.material_.data().pbr_;
                ImGui::ColorEdit3   ("Color"            , glm::value_ptr(data.color_));
                ImGui::InputFloat   ("Texture tiling X" , &data.tiling_x_);
                ImGui::InputFloat   ("Texture tiling Y" , &data.tiling_y_);
                ImGui::SliderFloat  ("Use albedo map"   , &data.use_albedo_map_ , 0.0f, 1.0f);
                ImGui::SliderFloat  ("Use PBR maps"     , &data.use_pbr_maps_   , 0.0f, 1.0f);
                ImGui::SliderFloat  ("Roughness"        , &data.roughness_      , 0.0f, 1.0f);
                ImGui::SliderFloat  ("Metallic"         , &data.metallic_       , 0.0f, 1.0f);
                ImGui::SliderFloat  ("Reflectance"      , &data.reflectance_    , 0.0f, 1.0f);
                dw.material_.set_data(data);
            }
        }
		ImGui::End();
	}

    void ImguiTools::resourceInspector()
    {
        static bool show = true;
        static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | 
            ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter |
            ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable |
            ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
        if (!ImGui::Begin("Resource inspector", &show, 0))
        {
            // Early out if the window is collapsed, as an optimization.
            ImGui::End();
            return;
        }

        Renderer &r = GM.renderer();
        if (ImGui::CollapsingHeader("Textures"))
        {
            ImVec2 size = ImVec2(-FLT_MIN, ImGui::GetTextLineHeightWithSpacing() * 6);
            if (ImGui::BeginTable("##table1", 2, flags, size))
            {
                ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
                ImGui::TableSetupColumn("Index", ImGuiTableColumnFlags_None);
                ImGui::TableSetupColumn("State", ImGuiTableColumnFlags_None);
                ImGui::TableHeadersRow();

                for (int32_t i = 0; i < r.tex_count_; ++i)
                {
                    const InternalTexture &t = r.textures_[i];
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%d", i);
                    ImGui::TableSetColumnIndex(1);
                    if (t.internal_id_ == CommonDefs::UNINIT_INTERNAL_ID)
                    {
                        ImGui::Text("Deleted");
                    }
                    else if (t.gpu_version_ == CommonDefs::DELETED_GPU_RESOURCE)
                    {
                        ImGui::Text("Marked for deletion");
                    }
                    else if (t.gpu_version_ < t.cpu_version_)
                    {
                        ImGui::Text("Outdated");
                    }
                    else if (t.cpu_version_ == t.gpu_version_)
                    {
                        ImGui::Text("Updated");
                    }
                }
                ImGui::EndTable();
            }
        }

        if (ImGui::CollapsingHeader("Buffers"))
        {
            ImVec2 size = ImVec2(-FLT_MIN, ImGui::GetTextLineHeightWithSpacing() * 6);
            if (ImGui::BeginTable("##table1", 2, flags, size))
            {
                ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
                ImGui::TableSetupColumn("Index", ImGuiTableColumnFlags_None);
                ImGui::TableSetupColumn("State", ImGuiTableColumnFlags_None);
                ImGui::TableHeadersRow();

                for (int32_t i = 0; i < r.buf_count_; ++i)
                {
                    const InternalBuffer &b = r.buffers_[i];
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%d", i);
                    ImGui::TableSetColumnIndex(1);
                    if (b.internal_id_ == CommonDefs::UNINIT_INTERNAL_ID)
                    {
                        ImGui::Text("Not created");
                    }
                    else if (b.gpu_version_ == CommonDefs::DELETED_GPU_RESOURCE)
                    {
                        ImGui::Text("Marked for deletion");
                    }
                    else if (b.gpu_version_ < b. cpu_version_)
                    {
                        ImGui::Text("Outdated");
                    }
                    else if (b.cpu_version_ == b.gpu_version_)
                    {
                        ImGui::Text("Updated");
                    }
                }
                ImGui::EndTable();
            }
        }
		ImGui::End();
    }

    void ImguiTools::memoryInspector()
    {
        static bool show = true;
        static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | 
            ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter |
            ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable |
            ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
        Memory &m = GM.memory();
        Renderer &r = GM.renderer();
        if (!ImGui::Begin("Memory inspector", &show, 0))
        {
            // Early out if the window is collapsed, as an optimization.
            ImGui::End();
            return;
        }

        ImVec2 size = ImVec2(-FLT_MIN, ImGui::GetTextLineHeightWithSpacing() * 6);
        if (ImGui::BeginTable("##table1", 4, flags, size))
        {
            ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
            ImGui::TableSetupColumn("Pool", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Used", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Capacity", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Percent", ImGuiTableColumnFlags_None);
            ImGui::TableHeadersRow();

            // Initial big chunk of memory
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", "Pools");
            ImGui::TableSetColumnIndex(1);
            // mem and offset are (int8_t*) so no need of sizeof here
            ImGui::Text("%.2f mb", ByteToMega(m.offset_ - m.mem_));
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%.2f mb", ByteToMega(kTotalMemSize));
            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%.1f %%", ((m.offset_-m.mem_)/(float)kTotalMemSize) * 100.0f);

            // Textures
            {
                size_t offset = r.tex_count_ * sizeof(InternalTexture);
                size_t capacity = kMaxTextures * sizeof(InternalTexture);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%s", "Textures");
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%.2f kb", ByteToKilo(offset));
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%.2f kb", ByteToKilo(capacity));
                ImGui::TableSetColumnIndex(3);
                ImGui::Text("%.1f %%", (offset/(float)capacity) * 100.0f);
            }

            // Buffers
            {
                size_t offset = r.buf_count_ * sizeof(InternalBuffer);
                size_t capacity = kMaxBuffers * sizeof(InternalBuffer);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%s", "Buffers");
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%.2f kb", ByteToKilo(offset));
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%.2f kb", ByteToKilo(capacity));
                ImGui::TableSetColumnIndex(3);
                ImGui::Text("%.1f %%", (offset/(float)capacity) * 100.0f);
            }

            // Materials
            // This will always full because the exact amount of materials is
            // known and does not change
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", "Materials");
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%.2f kb", ByteToKilo(kMatPoolSize));
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%.2f kb", ByteToKilo(kMatPoolSize));
            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%.1f %%", 100.0f);

            // Command pool (multiplied by 2 because there are 2 pools that
            // swap each frame (current and next))
            {
                // pool and offset are (int8_t*) so no need of sizeof here
                size_t offset = (r.rq_.next_offset_ - r.rq_.next_pool_) * 2;
                size_t capacity = kRenderPoolSize * 2;
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%s", "Command pool");
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%.2f kb", ByteToKilo(offset));
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%.2f kb", ByteToKilo(capacity));
                ImGui::TableSetColumnIndex(3);
                ImGui::Text("%.1f %%", (offset / (float)capacity) * 100.0f);
            }

            // Render queue
            {
                size_t offset = r.rq_.curr_count_ * sizeof(DLComm*) * 2;
                size_t capacity = kRenderQueueMaxCount * sizeof(DLComm*);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%s", "Render queue");
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%.2f kb", ByteToKilo(offset));
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%.2f kb", ByteToKilo(capacity));
                ImGui::TableSetColumnIndex(3);
                ImGui::Text("%.1f %%", (offset / (float)capacity) * 100.0f);
            }

            // Buddy allocator
            {
                // mem and offset are (int8_t*) so no need of sizeof here
                size_t offset = m.buddy_.mem_offset_ - m.buddy_.mem_;
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%s", "Buddy allocator");
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%.2f mb", ByteToMega(offset));
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%.2f mb", ByteToKilo(kMaxAlloc));
                ImGui::TableSetColumnIndex(3);
                ImGui::Text("%.1f %%", (offset / (float)kMaxAlloc) * 100.0f);
            }

            ImGui::EndTable();
        }
         

        ImGui::End();
    }
}