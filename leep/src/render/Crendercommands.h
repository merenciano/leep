#ifndef __THE_RENDER_COMMANDS_H__
#define __THE_RENDER_COMMANDS_H__

#include <stdint.h>

#include "render/buffer.h"
#include "render/renderer.h"
#include "render/framebuffer.h"
#include "core/manager.h"

enum THE_CommandDataType
{
	THE_CDT_CLEAR,
	THE_CDT_CREATE_BUFFER,
        THE_CDT_CREATE_CUBEMAP,
	THE_CDT_CREATE_FRAMEBUFFER,
	THE_CDT_CREATE_TEXTURE,
	THE_CDT_SKYBOX,
	THE_CDT_DRAW,
};

struct THE_ClearCommandData
{
	float color[4];
	int8_t bcolor;
	int8_t bdepth;
	int8_t bstencil;
};

struct THE_CreateBufferCommandData
{
	leep::Buffer buffer;
};

struct THE_CreateCubemapCommandData
{
        leep::Texture texture;
};

// TODO: Hide framebuffer concept from API
struct THE_CreateFramebufferCommandData
{
	leep::Framebuffer fb;
};

struct THE_CreateTextureCommandData
{
	leep::Texture tex;
	int32_t release_ram;
};

struct THE_SkyboxCommandData
{
	leep::Texture cubemap;
};

struct THE_DrawCommandData
{
	leep::Geometry geometry;
	leep::Material mat;
	leep::Buffer inst_attr;
	uint32_t inst_count;
};

union THE_CommandData
{
	THE_ClearCommandData clear;
	THE_CreateBufferCommandData createbuff;
        THE_CreateCubemapCommandData createcubemap;
	THE_CreateFramebufferCommandData createfb;
	THE_CreateTextureCommandData createtex;
	THE_SkyboxCommandData skybox;
	THE_DrawCommandData draw;
};

struct THE_RenderCommand
{
	void (*execute)(THE_CommandData *data);
	THE_RenderCommand *next;
	THE_CommandData data;
	THE_CommandDataType type;
};

void THE_ClearExecute(THE_CommandData *data);

void THE_CreateBufferExecute(THE_CommandData *data);

void THE_CreateCubemapExecute(THE_CommandData *data);

void THE_CreateFramebufferExecute(THE_CommandData *data);

void THE_CreateTextureExecute(THE_CommandData *data);

void THE_SkyboxExecute(THE_CommandData *data);

void THE_DrawExecute(THE_CommandData *data);

#endif
