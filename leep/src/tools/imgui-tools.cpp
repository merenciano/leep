#include "imgui-tools.h"

#include "core/Cmanager.h"
#include "tools/lua-scripting.h"
#include "tools/resource-map.h"

#include "core/io.h"
#include "core/Cmem.h"

#include "ecs/Centity.h"

#include "render/Crendercommands.h"
#include "render/Crenderer.h"
#include "render/Cinternalresources.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_opengl3.h> // TODO: implement it using Leep renderer
#include <ImGui/imgui_impl_glfw.h>

static s32 selected_entity = -1;
static bool show_components = false;
static bool show_lua_commands = false;
static bool show_entity_inspector = false;
static bool show_resource_inspector = false;
static bool show_memory_usage = false;
void (*callback)(void) = NULL;
THE_ToolsData tools_data;

static void THE_UIToolsInfoWindow(bool *show)
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Basic APP Info", show, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysAutoResize)) {
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Inspect")) {
			ImGui::MenuItem("Entities",   NULL, &show_entity_inspector);
			ImGui::MenuItem("Components", NULL, &show_components);
			ImGui::MenuItem("Resources",  NULL, &show_resource_inspector);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Memory")) {
			ImGui::MenuItem("Memory usage", NULL, &show_memory_usage);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Tools")) {
			ImGui::MenuItem("Commands", NULL, &show_lua_commands);
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::Text("Frame time: %f", THE_DeltaTime() * 1000.0f);
	ImGui::Text("FPS: %f", 1.0f / THE_DeltaTime());
	ImGui::Text("Init time (ms): %f", tools_data.init_time_ms);
	ImGui::Text("Logic average (ms): %f", tools_data.logic_average_ms);
	ImGui::Text("Render average (ms): %f", tools_data.render_average_ms);
	ImGui::PopItemWidth();
	ImGui::End();
}

static void THE_UIToolsLuaCommands()
{
	static bool show = true;
	ImGui::SetNextWindowPos(ImVec2(240, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 80), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Insert Lua Commands", &show, 0)) {
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	static char buffer[64];
	ImGui::InputText("Command input", buffer, IM_ARRAYSIZE(buffer));
	if (ImGui::Button("Execute")) {
		THE_ScriptingCommand(buffer);
	}
	ImGui::End();
}

static void THE_UIToolsEntityInspector()
{
	ImGui::SetNextWindowPos(ImVec2(10, 150), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Entity inspector", &show_entity_inspector, ImGuiWindowFlags_AlwaysAutoResize)) {
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

    for (u32 i = 0; i < THE_EntitiesSize(); ++i) {
        char tree_name[256];
        sprintf(tree_name, "Entity %d", i);
        if (ImGui::TreeNode(tree_name)) {
            static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
            ImVec2 size = ImVec2(-FLT_MIN, ImGui::GetTextLineHeightWithSpacing() * 12);
            if (ImGui::BeginTable("##table1", 2, flags, size)) {
                ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
                ImGui::TableSetupColumn("Index", ImGuiTableColumnFlags_None);
                ImGui::TableSetupColumn("Entity name", ImGuiTableColumnFlags_None);
                ImGui::TableHeadersRow();

                for (s32 j = 0; j < 1; ++j) {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%d", i);
                    ImGui::TableSetColumnIndex(1);
                    if (ImGui::Button("Entity")) {
                        show_components  = true;
                        selected_entity = i;
                    }
                }
                ImGui::EndTable();
            }
            ImGui::TreePop();
        }
    }
	ImGui::End();
}

static void THE_UIToolsComponentInspector()
{
	char window_title[64];
	if (selected_entity == -1) {
		return;
	}

	ImGui::SetNextWindowPos(ImVec2(185, 150), ImGuiCond_FirstUseEver);
	snprintf(window_title, 64, "Components of entity %d", selected_entity);
	if (!ImGui::Begin(window_title, &show_components, ImGuiWindowFlags_AlwaysAutoResize)) {
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	/*
	  Generating the entity from the name because the Entity type
	  can lose its reference if any entity is removed because
	  when that happens the last entity of the container is relocated
	  in order to fill the gap, so that last entity could be the one
	  stored in the entity variable and Entity::index_ would cause
	  a crash in the best case
	*/
    THE_Entity *e = THE_GetEntities() + selected_entity;
    if (e->mat.type == THE_MT_PBR) {
        THE_PbrData *data = (THE_PbrData*)e->mat.data;
        ImGui::ColorEdit3   ("Color"            , (float*)(&data->color));
        ImGui::InputFloat   ("Texture tiling X" , &data->tiling_x);
        ImGui::InputFloat   ("Texture tiling Y" , &data->tiling_y);
        ImGui::SliderFloat  ("Use albedo map"   , &data->use_albedo_map , 0.0f, 1.0f);
        ImGui::SliderFloat  ("Use PBR maps"     , &data->use_pbr_maps   , 0.0f, 1.0f);
        ImGui::SliderFloat  ("Roughness"        , &data->roughness      , 0.0f, 1.0f);
        ImGui::SliderFloat  ("Metallic"         , &data->metallic       , 0.0f, 1.0f);
        ImGui::SliderFloat  ("NormalMap intensity", &data->normal_map_intensity, 0.0f, 1.0f);
	}
	ImGui::End();
}

static void THE_UIToolsResourceInspector()
{
	static bool show = true;
	static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | 
		ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter |
		ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable |
		ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
	ImGui::SetNextWindowPos(ImVec2(1050, 10), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Resource inspector", &show, ImGuiWindowFlags_AlwaysAutoResize)) {
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	if (ImGui::CollapsingHeader("Textures")) {
		ImVec2 size = ImVec2(-FLT_MIN, ImGui::GetTextLineHeightWithSpacing() * 6);
		if (ImGui::BeginTable("##table1", 2, flags, size)) {
			ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
			ImGui::TableSetupColumn("Index", ImGuiTableColumnFlags_None);
			ImGui::TableSetupColumn("State", ImGuiTableColumnFlags_None);
			ImGui::TableHeadersRow();

			for (s32 i = 0; i < texture_count; ++i) {
				THE_InternalTexture *t = textures + i;
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("%d", i);
				ImGui::TableSetColumnIndex(1);
				if (t->internal_id == THE_UNINIT) {
					ImGui::Text("Not created");
				} else if (t->gpu_version == THE_DELETED) {
					ImGui::Text("Marked for deletion");
				} else if (t->gpu_version < t->cpu_version) {
					ImGui::Text("Outdated");
				} else if (t->cpu_version == t->gpu_version) {
					ImGui::Text("Updated");
				}
			}
			ImGui::EndTable();
		}
	}

	if (ImGui::CollapsingHeader("Buffers")) {
		ImVec2 size = ImVec2(-FLT_MIN, ImGui::GetTextLineHeightWithSpacing() * 6);
		if (ImGui::BeginTable("##table1", 2, flags, size)) {
			ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
			ImGui::TableSetupColumn("Index", ImGuiTableColumnFlags_None);
			ImGui::TableSetupColumn("State", ImGuiTableColumnFlags_None);
			ImGui::TableHeadersRow();

			for (s32 i = 0; i < buffer_count; ++i) {
				THE_InternalBuffer *b = buffers + i;
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("%d", i);
				ImGui::TableSetColumnIndex(1);
				if (b->internal_id == THE_UNINIT) {
					ImGui::Text("Not created");
				} else if (b->gpu_version == THE_DELETED) {
					ImGui::Text("Marked for deletion");
				} else if (b->gpu_version < b->cpu_version) {
					ImGui::Text("Outdated");
				} else if (b->cpu_version == b->gpu_version) {
					ImGui::Text("Updated");
				}
			}
			ImGui::EndTable();
		}
	}
	ImGui::End();
}

static void THE_UIToolsMemoryUsage()
{
	static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | 
		ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter |
		ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable |
		ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
	ImGui::SetNextWindowPos(ImVec2(10, 560), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Memory usage", &show_memory_usage, ImGuiWindowFlags_AlwaysAutoResize)) {
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImVec2 size = ImVec2(-FLT_MIN, ImGui::GetTextLineHeightWithSpacing() * 7);
	if (ImGui::BeginTable("##table1", 4, flags, size)) {
		ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
		ImGui::TableSetupColumn("Pool", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Used", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Capacity", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Percent", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableHeadersRow();

		// Initial big chunk of memory
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("%s", "Leep memory");
		ImGui::TableSetColumnIndex(1);
		ImGui::Text("%.2f MB", THE_BYTE_TO_MB(THE_MemUsedBytes()));
		ImGui::TableSetColumnIndex(2);
		ImGui::Text("%.2f MB", THE_BYTE_TO_MB(THE_MemCapacity()));
		ImGui::TableSetColumnIndex(3);
		ImGui::Text("%.1f %%", (THE_MemUsedBytes()/(float)THE_MemCapacity()) * 100.0f);

		// Textures
		{
			size_t offset = texture_count * sizeof(THE_InternalTexture);
			size_t capacity = kMaxTextures * sizeof(THE_InternalTexture);
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%s", "Textures");
			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%.2f KB", THE_BYTE_TO_KB(offset));
			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%.2f KB", THE_BYTE_TO_KB(capacity));
			ImGui::TableSetColumnIndex(3);
			ImGui::Text("%.1f %%", (offset/(float)capacity) * 100.0f);
		}

		// Buffers
		{
			size_t offset = buffer_count * sizeof(THE_InternalBuffer);
			size_t capacity = kMaxBuffers * sizeof(THE_InternalBuffer);
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%s", "Buffers");
			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%.2f KB", THE_BYTE_TO_KB(offset));
			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%.2f KB", THE_BYTE_TO_KB(capacity));
			ImGui::TableSetColumnIndex(3);
			ImGui::Text("%.1f %%", (offset/(float)capacity) * 100.0f);
		}

		// Materials
		// This will always full because the exact amount of materials is
		/* known and does not change
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("%s", "Materials");
		ImGui::TableSetColumnIndex(1);
		ImGui::Text("%.2f B", (float)kMatPoolSize);
		ImGui::TableSetColumnIndex(2);
		ImGui::Text("%.2f B", (float)kMatPoolSize);
		ImGui::TableSetColumnIndex(3);
		ImGui::Text("%.1f %%", 100.0f);*/

		{
			// pool and offset are (int8_t*) so no need of sizeof here
			size_t offset = THE_RenderQueueUsed() * sizeof(THE_RenderCommand);
			size_t capacity = THE_RENDER_QUEUE_CAPACITY * sizeof(THE_RenderCommand);
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%s", "Command pool");
			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%.2f KB", THE_BYTE_TO_KB(offset));
			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%.2f KB", THE_BYTE_TO_KB(capacity));
			ImGui::TableSetColumnIndex(3);
			ImGui::Text("%.1f %%", (offset / (float)capacity) * 100.0f);
		}

		/* Render queue
		{
		    size_t offset = (render_queue->curr_last - render_queue->curr) * sizeof(THE_RenderCommand*);
		    size_t capacity = (render_queue->curr_last - render_queue->curr) * sizeof(THE_RenderCommand*);
		    ImGui::TableNextRow();
		    ImGui::TableSetColumnIndex(0);
		    ImGui::Text("%s", "Render queue");
		    ImGui::TableSetColumnIndex(1);
		    ImGui::Text("%.2f KB", ByteToKilo(offset));
		    ImGui::TableSetColumnIndex(2);
		    ImGui::Text("%.2f KB", ByteToKilo(capacity));
		    ImGui::TableSetColumnIndex(3);
		    ImGui::Text("%.1f %%", (offset / (float)capacity) * 100.0f);
		}*/

		// Buddy allocator TODO: descomentar cuando este implementado el buddy.
		/*{
			// mem and offset are (int8_t*) so no need of sizeof here
			size_t used = m.buddy_.mem_used_;
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%s", "General alloc");
			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%.2f MB", ByteToMega(used));
			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%.2f MB", ByteToMega(leep::kMaxAlloc));
			ImGui::TableSetColumnIndex(3);
			ImGui::Text("%.1f %%", (used / (float)leep::kMaxAlloc) * 100.0f);
		}*/

		ImGui::EndTable();
	}    

	ImGui::End();
}

void THE_UIToolsInit(void *raw_window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)raw_window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void THE_UIToolsSetCallback(void(*callbackfun)(void))
{
	callback = callbackfun;
}

void THE_UIToolsUpdate()
{
	static bool show_info = true;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGuiIO& io = ImGui::GetIO();
	// TODO: One frame late? Check carefully how it's working
	// and improve if necessary
	THE_InputCapture(!io.WantCaptureMouse, !io.WantCaptureKeyboard);

	if (callback) {
	    (*callback)();
	}

	static bool show_demo = false;
	if (show_demo)
	    ImGui::ShowDemoWindow(&show_demo);

	THE_UIToolsInfoWindow(&show_info);
	if (show_lua_commands) {
		THE_UIToolsLuaCommands();
	}

	if (show_entity_inspector) {
		THE_UIToolsEntityInspector();
	}

	if (show_components) {
		THE_UIToolsComponentInspector();
	}

	if (show_resource_inspector) {
		THE_UIToolsResourceInspector();
	}

	if (show_memory_usage) {
		THE_UIToolsMemoryUsage();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void THE_UIToolsCalcLogicAverage(float frame)
{
	static s32 i = 0;
	static float previous[32];
	previous[i++] = frame;
	if (i == 32) {
		float avg = 0.0f;
		i = 0;
		for (int j = 0; j < 32; ++j) {
			avg += previous[j];
		}

		tools_data.logic_average_ms = avg / 32.0f;
	}
}

void THE_UIToolsCalcRenderAverage(float frame)
{
	static s32 i = 0;
	static float previous[64];
	previous[i++] = frame;
	if (i == 64) {
		float avg = 0.0f;
		i = 0;
		for (s32 j = 0; j < 64; ++j) {
			avg += previous[j];
		}

		tools_data.render_average_ms = avg / 32.0f;
	}
}

bool THE_UIToolsWantKeyboard()
{
	ImGuiIO &io = ImGui::GetIO();
	return io.WantCaptureKeyboard;
}

bool THE_UIToolsWantMouse()
{
	ImGuiIO &io = ImGui::GetIO();
	return io.WantCaptureMouse;
}
