#include "imgui-tools.h"
#include "core/manager.h"
#include "core/memory/memory.h"
#include "ecs/entity.h"
#include "tools/lua-scripting.h"

#include <vector>
#include <deque>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_opengl3.h> // TODO(Lucas): implement myself
#include <ImGui/imgui_impl_glfw.h>
#include <GLFW/glfw3.h> // remove this when implementing input

namespace leep
{
    static void BasicAppInfo();
    static void LuaCommands();
    static void EntityInspector();

    ImguiTools::ImguiTools()
    {

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
        EntityInspector();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImguiTools::render()
    {

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

    static void EntityInspector()
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
                for (uint32_t i = 0; i < it->second.chunks_.size(); ++i)
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

                            for (int32_t j = 0; j < it->second.chunks_[i]->last_; ++j)
                            {
                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::Text("%d", j);
                                ImGui::TableSetColumnIndex(1);
                                ImGui::Text("%s", Entity(j, it->first).name().c_str());
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
}