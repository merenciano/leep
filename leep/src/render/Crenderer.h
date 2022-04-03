#ifndef __THE_RENDER_RENDERER_H__
#define __THE_RENDER_RENDERER_H__

#include "Cbuffer.h"

#define THE_RENDER_QUEUE_CAPACITY 12000
#define THE_FRAME_POOL_SIZE 1048576

typedef int32_t THE_Texture;
typedef int32_t THE_Buffer;
typedef int32_t THE_Geometry;
typedef int32_t THE_Material;

struct THE_RenderCommand;

typedef struct {
	THE_RenderCommand *curr;
	THE_RenderCommand *curr_last;
	THE_RenderCommand *next;
	THE_RenderCommand *next_last;
} THE_RenderQueue;

extern THE_RenderQueue render_queue;

THE_Buffer THE_CreateBuffer();
THE_Buffer THE_CreateBuffer(float *data, uint32_t count, THE_BufferType type);

void THE_InitRender();

void THE_AddCommands(THE_RenderCommand *rc);

void THE_RenderFrame();

void THE_SubmitFrame();

THE_RenderCommand *THE_AllocateCommand();

void *THE_AllocateFrameResource(size_t size);

int32_t THE_IsInsideFramePool(void *address);

size_t THE_RenderQueueUsed();

#endif