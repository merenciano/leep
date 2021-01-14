#include "imgui-tools.h"
#include "core/manager.h"
#include "tools/lua-scripting.h"

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

    void ImguiTools::update(float delta_time, int32_t width, int32_t height)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize = ImVec2(width, height);
        io.DeltaTime = delta_time;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        BasicAppInfo();
        LuaCommands();


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
}