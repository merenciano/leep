#include "imgui-tools.h"

#include "core/manager.h"
#include "tools/lua-scripting.h"
#include "tools/resource-map.h"

#include "core/io.h"
#include "core/mem.h"

#include "ecs/entity.h"

#include "render/rendercommands.h"
#include "render/renderer.h"
#include "render/internalresources.h"

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui.h"
#include "cimgui_impl.h"

static int32_t selected_entity = -1;
static bool show_components = false;
static bool show_lua_commands = false;
static bool show_entity_inspector = false;
static bool show_resource_inspector = false;
static bool show_memory_usage = false;
static ImGuiIO *io = NULL;
void (*callback)(void) = NULL;
THE_ToolsData tools_data;

static ImVec2 CImVec2(float x, float y)
{
    ImVec2 v = {x, y};
    return v;
}

static void THE_UIToolsInfoWindow(bool *show)
{
	igSetNextWindowPos(CImVec2(10.0f, 10.0f), ImGuiCond_FirstUseEver, CImVec2(0.0f, 0.0f));

	if (!igBegin("Basic APP Info", show, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysAutoResize)) {
		// Early out if the window is collapsed, as an optimization.
		igEnd();
		return;
	}

	igPushItemWidth(igGetFontSize() * -12);
	if (igBeginMenuBar()) {
		if (igBeginMenu("Inspect", true)) {
			igMenuItem_BoolPtr("Entities",   NULL, &show_entity_inspector, true);
			igMenuItem_BoolPtr("Components", NULL, &show_components, true);
			igMenuItem_BoolPtr("Resources",  NULL, &show_resource_inspector, true);
			igEndMenu();
		}

		if (igBeginMenu("Memory", true)) {
			igMenuItem_BoolPtr("Memory usage", NULL, &show_memory_usage, true);
			igEndMenu();
		}

		if (igBeginMenu("Tools", true)) {
			igMenuItem_BoolPtr("Commands", NULL, &show_lua_commands, true);
			igEndMenu();
		}

		igEndMenuBar();
	}

	igText("Frame time: %f", THE_DeltaTime() * 1000.0f);
	igText("FPS: %f", 1.0f / THE_DeltaTime());
	igText("Init time (ms): %f", tools_data.init_time_ms);
	igText("Logic average (ms): %f", tools_data.logic_average_ms);
	igText("Render average (ms): %f", tools_data.render_average_ms);
	igPopItemWidth();
	igEnd();
}

static void THE_UIToolsLuaCommands()
{
	static bool show = true;
	igSetNextWindowPos(CImVec2(240.0f, 10.0f), ImGuiCond_FirstUseEver, CImVec2(0.0f, 0.0f));
	igSetNextWindowSize(CImVec2(300.0f, 80.0f), ImGuiCond_FirstUseEver);
	if (!igBegin("Insert Lua Commands", &show, 0)) {
		// Early out if the window is collapsed, as an optimization.
		igEnd();
		return;
	}

	static char buffer[64];
	igInputText("Command input", buffer, sizeof(buffer), 0, NULL, NULL);
	if (igButton("Execute", CImVec2(0.0f, 0.0f))) {
		THE_ScriptingCommand(buffer);
	}
	igEnd();
}

static void THE_UIToolsEntityInspector()
{
	igSetNextWindowPos(CImVec2(10.0f, 150.0f), ImGuiCond_FirstUseEver, CImVec2(0.0f, 0.0f));
	if (!igBegin("Entity inspector", &show_entity_inspector, ImGuiWindowFlags_AlwaysAutoResize)) {
		// Early out if the window is collapsed, as an optimization.
		igEnd();
		return;
	}

    for (u32 i = 0; i < THE_EntitiesSize(); ++i) {
        char tree_name[256];
        sprintf(tree_name, "Entity %d", i);
        if (igTreeNode_Str(tree_name)) {
            static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
            ImVec2 size = {-FLT_MIN, igGetTextLineHeightWithSpacing() * 12};
            if (igBeginTable("##table1", 2, flags, size, 0.0f)) {
                igTableSetupScrollFreeze(0, 1); // Make top row always visible
                igTableSetupColumn("Index", ImGuiTableColumnFlags_None, 0.0f, 0);
                igTableSetupColumn("Entity name", ImGuiTableColumnFlags_None, 0.0f, 0);
                igTableHeadersRow();

                for (s32 j = 0; j < 1; ++j) {
                    igTableNextRow(0, 0.0f);
                    igTableSetColumnIndex(0);
                    igText("%d", i);
                    igTableSetColumnIndex(1);
                    if (igButton("Entity", CImVec2(0.0f, 0.0f))) {
                        show_components  = true;
                        selected_entity = i;
                    }
                }
                igEndTable();
            }
            igTreePop();
        }
    }
	igEnd();
}

static void THE_UIToolsComponentInspector()
{
	char window_title[64];
	if (selected_entity == -1) {
		return;
	}

	igSetNextWindowPos(CImVec2(185.0f, 150.0f), ImGuiCond_FirstUseEver, CImVec2(0.0f, 0.0f));
	snprintf(window_title, 64, "Components of entity %d", selected_entity);
	if (!igBegin(window_title, &show_components, ImGuiWindowFlags_AlwaysAutoResize)) {
		// Early out if the window is collapsed, as an optimization.
		igEnd();
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
        igColorEdit3   ("Color"            , (float*)(&data->color), 0);
        igInputFloat   ("Texture tiling X" , &data->tiling_x, 0.0f, 0.0f, "%.3f", 0);
        igInputFloat   ("Texture tiling Y" , &data->tiling_y, 0.0f, 0.0f, "%.3f", 0);
        igSliderFloat  ("Use albedo map"   , &data->use_albedo_map , 0.0f, 1.0f, "%.3f", 0);
        igSliderFloat  ("Use PBR maps"     , &data->use_pbr_maps   , 0.0f, 1.0f, "%.3f", 0);
        igSliderFloat  ("Roughness"        , &data->roughness      , 0.0f, 1.0f, "%.3f", 0);
        igSliderFloat  ("Metallic"         , &data->metallic       , 0.0f, 1.0f, "%.3f", 0);
        igSliderFloat  ("NormalMap intensity", &data->normal_map_intensity, 0.0f, 1.0f, "%.3f", 0);
	}
	igEnd();
}

static void THE_UIToolsResourceInspector()
{
	static bool show = true;
	static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | 
		ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter |
		ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable |
		ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
	igSetNextWindowPos(CImVec2(1050.0f, 10.0f), ImGuiCond_FirstUseEver, CImVec2(0.0f, 0.0f));
	if (!igBegin("Resource inspector", &show, ImGuiWindowFlags_AlwaysAutoResize)) {
		// Early out if the window is collapsed, as an optimization.
		igEnd();
		return;
	}

	if (igCollapsingHeader_TreeNodeFlags("Textures", 0)) {
		ImVec2 size = {-FLT_MIN, igGetTextLineHeightWithSpacing() * 6};
		if (igBeginTable("##table1", 2, flags, size, 0.0f)) {
			igTableSetupScrollFreeze(0, 1); // Make top row always visible
			igTableSetupColumn("Index", ImGuiTableColumnFlags_None, 0.0f, 0);
			igTableSetupColumn("State", ImGuiTableColumnFlags_None, 0.0f, 0);
			igTableHeadersRow();

			for (s32 i = 0; i < texture_count; ++i) {
				THE_InternalTexture *t = textures + i;
				igTableNextRow(0, 0.0f);
				igTableSetColumnIndex(0);
				igText("%d", i);
				igTableSetColumnIndex(1);
				if (t->internal_id == THE_UNINIT) {
					igText("Not created");
				} else if (t->gpu_version == THE_DELETED) {
					igText("Marked for deletion");
				} else if (t->gpu_version < t->cpu_version) {
					igText("Outdated");
				} else if (t->cpu_version == t->gpu_version) {
					igText("Updated");
				}
			}
			igEndTable();
		}
	}

	if (igCollapsingHeader_TreeNodeFlags("Buffers", 0)) {
		ImVec2 size = {-FLT_MIN, igGetTextLineHeightWithSpacing() * 6};
		if (igBeginTable("##table1", 2, flags, size, 0.0f)) {
			igTableSetupScrollFreeze(0, 1); // Make top row always visible
			igTableSetupColumn("Index", ImGuiTableColumnFlags_None, 0.0f, 0);
			igTableSetupColumn("State", ImGuiTableColumnFlags_None, 0.0f, 0);
			igTableHeadersRow();

			for (s32 i = 0; i < buffer_count; ++i) {
				THE_InternalBuffer *b = buffers + i;
				igTableNextRow(0, 0.0f);
				igTableSetColumnIndex(0);
				igText("%d", i);
				igTableSetColumnIndex(1);
				if (b->internal_id == THE_UNINIT) {
					igText("Not created");
				} else if (b->gpu_version == THE_DELETED) {
					igText("Marked for deletion");
				} else if (b->gpu_version < b->cpu_version) {
					igText("Outdated");
				} else if (b->cpu_version == b->gpu_version) {
					igText("Updated");
				}
			}
			igEndTable();
		}
	}
	igEnd();
}

static void THE_UIToolsMemoryUsage()
{
	static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | 
		ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter |
		ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable |
		ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
	igSetNextWindowPos(CImVec2(10, 560), ImGuiCond_FirstUseEver, CImVec2(0, 0));
	if (!igBegin("Memory usage", &show_memory_usage, ImGuiWindowFlags_AlwaysAutoResize)) {
		// Early out if the window is collapsed, as an optimization.
		igEnd();
		return;
	}

	ImVec2 size = {-FLT_MIN, igGetTextLineHeightWithSpacing() * 7};
	if (igBeginTable("##table1", 4, flags, size, 0.0f)) {
		igTableSetupScrollFreeze(0, 1); // Make top row always visible
		igTableSetupColumn("Pool", ImGuiTableColumnFlags_WidthStretch, 0.0f, 0);
		igTableSetupColumn("Used", ImGuiTableColumnFlags_WidthStretch, 0.0f, 0);
		igTableSetupColumn("Capacity", ImGuiTableColumnFlags_WidthStretch, 0.0f, 0);
		igTableSetupColumn("Percent", ImGuiTableColumnFlags_WidthStretch, 0.0f, 0);
		igTableHeadersRow();

		// Initial big chunk of memory
		igTableNextRow(0, 0.0f);
		igTableSetColumnIndex(0);
		igText("%s", "The memory");
		igTableSetColumnIndex(1);
		igText("%.2f MB", THE_BYTE_TO_MB(THE_MemUsedBytes()));
		igTableSetColumnIndex(2);
		igText("%.2f MB", THE_BYTE_TO_MB(THE_MemCapacity()));
		igTableSetColumnIndex(3);
		igText("%.1f %%", (THE_MemUsedBytes()/(float)THE_MemCapacity()) * 100.0f);

		// Textures
		{
			size_t offset = texture_count * sizeof(THE_InternalTexture);
			size_t capacity = MAX_TEXTURES * sizeof(THE_InternalTexture);
			igTableNextRow(0, 0.0f);
			igTableSetColumnIndex(0);
			igText("%s", "Textures");
			igTableSetColumnIndex(1);
			igText("%.2f KB", THE_BYTE_TO_KB(offset));
			igTableSetColumnIndex(2);
			igText("%.2f KB", THE_BYTE_TO_KB(capacity));
			igTableSetColumnIndex(3);
			igText("%.1f %%", (offset/(float)capacity) * 100.0f);
		}

		// Buffers
		{
			size_t offset = buffer_count * sizeof(THE_InternalBuffer);
			size_t capacity = MAX_BUFFERS * sizeof(THE_InternalBuffer);
			igTableNextRow(0, 0.0f);
			igTableSetColumnIndex(0);
			igText("%s", "Buffers");
			igTableSetColumnIndex(1);
			igText("%.2f KB", THE_BYTE_TO_KB(offset));
			igTableSetColumnIndex(2);
			igText("%.2f KB", THE_BYTE_TO_KB(capacity));
			igTableSetColumnIndex(3);
			igText("%.1f %%", (offset/(float)capacity) * 100.0f);
		}

		// Render queue
		{
			size_t offset = THE_RenderQueueUsed() * sizeof(THE_RenderCommand);
			size_t capacity = THE_RENDER_QUEUE_CAPACITY * sizeof(THE_RenderCommand);
			igTableNextRow(0, 0.0f);
			igTableSetColumnIndex(0);
			igText("%s", "Command pool");
			igTableSetColumnIndex(1);
			igText("%.2f KB", THE_BYTE_TO_KB(offset));
			igTableSetColumnIndex(2);
			igText("%.2f KB", THE_BYTE_TO_KB(capacity));
			igTableSetColumnIndex(3);
			igText("%.1f %%", (offset / (float)capacity) * 100.0f);
		}

		// Memory allocator
		{
			size_t used = THE_MemUsedBytes();
			igTableNextRow(0, 0.0f);
			igTableSetColumnIndex(0);
			igText("%s", "General alloc");
			igTableSetColumnIndex(1);
			igText("%.2f MB", THE_BYTE_TO_MB(used));
			igTableSetColumnIndex(2);
			igText("%.2f MB", THE_BYTE_TO_MB(THE_MemCapacity()));
			igTableSetColumnIndex(3);
			igText("%.1f %%", (used / (float)THE_MemCapacity()) * 100.0f);
		}

		igEndTable();
	}    

	igEnd();
}

void THE_UIToolsInit(void *raw_window)
{
	igDebugCheckVersionAndDataLayout(igGetVersion(), sizeof(ImGuiIO), sizeof(ImGuiStyle), sizeof(ImVec2), sizeof(ImVec4), sizeof(ImDrawVert), sizeof(ImDrawIdx));
	igCreateContext(NULL);
	igStyleColorsDark(NULL);
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
	igNewFrame();

	io = igGetIO();
	// TODO: One frame late? Check carefully how it's working
	// and improve if necessary
	THE_InputCapture(!io->WantCaptureMouse, !io->WantCaptureKeyboard);

	if (callback) {
	    (*callback)();
	}

	static bool show_demo = false;
	if (show_demo)
	    igShowDemoWindow(&show_demo);

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

	igRender();
	ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
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
	return io->WantCaptureKeyboard;
}

bool THE_UIToolsWantMouse()
{
	return io->WantCaptureMouse;
}
