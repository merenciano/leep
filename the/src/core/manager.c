#include "manager.h"
#include "chrono.h"
#include "io.h"
#include "mem.h"
#include "render/renderer.h"
#include "render/rendercommands.h"
#include "render/internalresources.h"
#include "tools/lua-scripting.h"

THE_ResourceMap resource_map;
static float delta_time;
static THE_Chrono frame_timer;

void THE_InitManager(THE_Config *cnfg)
{
	size_t total_mem = MAX_BUFFERS * sizeof(THE_InternalBuffer) + MAX_TEXTURES * sizeof(THE_InternalTexture);
	total_mem += THE_MT_MAX * sizeof(THE_InternalMaterial);
	total_mem += cnfg->render_queue_capacity * 2 * sizeof(void*); // 2 because current and next
	total_mem += cnfg->render_queue_capacity * 2 * sizeof(THE_CommandData);
	total_mem += cnfg->alloc_capacity;
	THE_MemInit(total_mem);
	THE_IOInit(WINDOW_TITLE, cnfg->window_width, cnfg->window_height, cnfg->vsync);
	resource_map = *(THE_ResourceMap*)THE_Alloc(sizeof(THE_ResourceMap));
	resource_map.meshes = THE_HMapCreate(8, sizeof(THE_Mesh));
	resource_map.textures = THE_HMapCreate(64, sizeof(THE_Texture));
	THE_InitRender();
	THE_ResourceMapAddMesh(&resource_map, "Cube", CUBE_MESH);
	THE_ResourceMapAddMesh(&resource_map, "Sphere", SPHERE_MESH);
	THE_ResourceMapAddMesh(&resource_map, "Quad", QUAD_MESH);
	delta_time = 0.16f;
}

void THE_NextFrame()
{
	THE_WindowSwapBuffers();
	THE_IOPollEvents();
	THE_SubmitFrame();

	THE_ChronoEnd(&frame_timer);
	delta_time = THE_ChronoDurationSec(&frame_timer);
	THE_ChronoStart(&frame_timer);

	THE_ScriptingSetGlobal("g_deltatime", delta_time);
}

void THE_StartFrameTimer()
{
	THE_ChronoStart(&frame_timer);
}

float THE_DeltaTime()
{
	return delta_time;
}