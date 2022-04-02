#ifndef __THE_RENDER_COMMANDS_H__
#define __THE_RENDER_COMMANDS_H__

#include <stdint.h>

#include "render/buffer.h"
#include "render/renderer.h"
#include "render/framebuffer.h"
#include "core/manager.h"

#define THE_BLEND_FUNC_BIT     1
#define THE_ENABLE_BLEND_BIT   1 << 1
#define THE_WRITE_DEPTH_BIT    1 << 2
#define THE_DEPTH_TEST_BIT     1 << 3
#define THE_CULL_FACE_BIT      1 << 4

typedef enum {
	THE_CDT_CLEAR,
	THE_CDT_CREATE_BUFFER,
	THE_CDT_CREATE_CUBEMAP,
	THE_CDT_CREATE_FRAMEBUFFER,
	THE_CDT_CREATE_TEXTURE,
	THE_CDT_SKYBOX,
	THE_CDT_DRAW,
	THE_CDT_EQUIREC_CUBEMAP,
} THE_CommandDataType;

typedef struct {
	float color[4];
	int8_t bcolor;
	int8_t bdepth;
	int8_t bstencil;
} THE_ClearCommandData;

typedef struct {
	leep::Buffer buffer;
} THE_CreateBufferCommandData;

typedef struct {
	leep::Texture texture;
} THE_CreateCubemapCommandData;

// TODO: Hide framebuffer concept from API
typedef struct {
	leep::Framebuffer fb;
} THE_CreateFramebufferCommandData;

typedef struct {
	leep::Texture tex;
	int32_t release_ram;
} THE_CreateTextureCommandData;

typedef struct {
	leep::Texture cubemap;
} THE_SkyboxCommandData;

typedef struct {
	leep::Geometry geometry;
	leep::Material *mat;
	leep::Buffer inst_attr;
	uint32_t inst_count;
} THE_DrawCommandData;

typedef struct {
	char in_path[128];
	leep::Texture out_cube;
	leep::Texture out_prefilt;
	leep::Texture out_lut;
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
	uint32_t changed_mask;
	THE_RenderOptions sfactor;
	THE_RenderOptions dfactor;
	THE_RenderOptions cull_face;
	int8_t depth_test;
	int8_t write_depth;
	int8_t blend;
} THE_RenderOptionsData;

typedef struct {
	leep::Framebuffer fb;
	int8_t def;
} THE_UseFramebufferData;

typedef struct {
	leep::Material mat;
} THE_UseMaterialData;

//typedef union {
typedef union THE_CommandData {
	// TODO: Quitar los putos construc y destruc para que no maree
	// el IDE pensando que es C++
	THE_CommandData() {};
	~THE_CommandData() {};
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
} THE_CommandData;// THE_CommandData;

//struct THE_RenderCommand {
typedef struct THE_RenderCommand {
	void (*execute)(THE_CommandData* data);
	THE_RenderCommand* next;
	THE_CommandData data;
	THE_CommandDataType type;
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
