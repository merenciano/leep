#ifndef __THE_RENDER_RENDERER_H__
#define __THE_RENDER_RENDERER_H__

#include "render/renderer.h"
struct THE_RenderCommand;

void THE_AddCommands(leep::Renderer *r, THE_RenderCommand *rc);

void THE_RenderFrame(leep::Renderer *r);

void THE_RenderSubmitFrame(leep::Renderer *r);

#endif