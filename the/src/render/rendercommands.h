#ifndef THE_RENDER_COMMANDS_H
#define THE_RENDER_COMMANDS_H

#include "renderer.h"

#define THE_BLEND_FUNC_BIT     1
#define THE_ENABLE_BLEND_BIT   1 << 1
#define THE_WRITE_DEPTH_BIT    1 << 2
#define THE_DEPTH_TEST_BIT     1 << 3
#define THE_CULL_FACE_BIT      1 << 4

typedef struct {
	float color[4];
	s8 bcolor;
	s8 bdepth;
	s8 bstencil;
} THE_ClearCommandData;

typedef struct {
	THE_Buffer buffer;
} THE_CreateBufferCommandData;

typedef struct {
	THE_Texture texture;
} THE_CreateCubemapCommandData;

// TODO: Hide framebuffer concept from API
typedef struct {
	THE_Framebuffer fb;
} THE_CreateFramebufferCommandData;

typedef struct {
	THE_Texture tex;
	s32 release_ram;
} THE_CreateTextureCommandData;

typedef struct {
	THE_Texture cubemap;
} THE_SkyboxCommandData;

typedef struct {
	THE_Mesh mesh;
	THE_Material *mat;
	THE_Buffer inst_attr;
	u32 inst_count;
} THE_DrawCommandData;

typedef struct {
	char in_path[128];
	THE_Texture out_cube;
	THE_Texture out_prefilt;
	THE_Texture out_lut;
} THE_EquirectToCubeData;

typedef enum {
	THE_BLENDFUNC_ONE,
	THE_BLENDFUNC_SRC_ALPHA,
	THE_BLENDFUNC_ONE_MINUS_SRC_ALPHA,
	THE_BLENDFUNC_ZERO,

	THE_CULLFACE_DISABLED,
	THE_CULLFACE_FRONT,
	THE_CULLFACE_BACK,
	THE_CULLFACE_FRONT_AND_BACK,
} THE_RenderOptions;

typedef struct {
	u32 changed_mask;
	THE_RenderOptions sfactor;
	THE_RenderOptions dfactor;
	THE_RenderOptions cull_face;
	s8 depth_test;
	s8 write_depth;
	s8 blend;
} THE_RenderOptionsData;

typedef struct {
	THE_Framebuffer fb;
	s8 def;
} THE_UseFramebufferData;

typedef struct {
	THE_Material *mat;
} THE_UseMaterialData;

typedef union THE_CommandData {
	THE_ClearCommandData clear;
	THE_CreateBufferCommandData createbuff;
	THE_CreateCubemapCommandData createcubemap;
	THE_CreateFramebufferCommandData createfb;
	THE_CreateTextureCommandData createtex;
	THE_SkyboxCommandData skybox;
	THE_DrawCommandData draw;
	THE_EquirectToCubeData eqr_cube;
	THE_RenderOptionsData renderops;
	THE_UseFramebufferData usefb;
	THE_UseMaterialData usemat;
} THE_CommandData;

typedef struct THE_RenderCommand {
	void (*execute)(THE_CommandData* data);
	THE_RenderCommand* next;
	THE_CommandData data;
} THE_RenderCommand;

void THE_ClearExecute(THE_CommandData *data);

void THE_CreateBufferExecute(THE_CommandData *data);

void THE_CreateCubemapExecute(THE_CommandData *data);

void THE_CreateFramebufferExecute(THE_CommandData *data);

void THE_CreateTextureExecute(THE_CommandData *data);

void THE_SkyboxExecute(THE_CommandData *data);

void THE_DrawExecute(THE_CommandData *data);

void THE_EquirectToCubeExecute(THE_CommandData *data);

void THE_RenderOptionsExecute(THE_CommandData *data);

void THE_UseFramebufferExecute(THE_CommandData *data);

void THE_UseMaterialExecute(THE_CommandData *data);

#endif
