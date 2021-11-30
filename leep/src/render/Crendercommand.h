#ifndef __THE_RENDER_COMMAND_H__
#define __THE_RENDER_COMMAND_H__

#include <stdint.h>

#include "render/buffer.h"
#include "render/renderer.h"
#include "core/manager.h"

enum THE_CommandDataType
{
	THE_CDT_CLEAR,
	THE_CDT_CREATE_BUFFER,
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

union THE_CommandData
{
	THE_ClearCommandData clear;
	THE_CreateBufferCommandData createbuff;
};

struct THE_RenderCommand
{
	void (*execute)(THE_CommandData *data);
	THE_RenderCommand *next;
	THE_CommandData data;
	THE_CommandDataType type;
};

#ifdef THE_OPENGL
// TODO: Cambiar esto por SDL y Opengl
//#include "SDL2/SDL.h"
//#include "SDL2/SDL_opengl.h"
//#include <GL/gl.h>
#include "glad/glad.h"

void THE_ClearExecute(THE_CommandData *data)
{
	uint32_t mask = 0U;
        if (data->clear.bcolor) mask |= GL_COLOR_BUFFER_BIT;
        if (data->clear.bdepth) mask |= GL_DEPTH_BUFFER_BIT;
        if (data->clear.bstencil) mask |= GL_STENCIL_BUFFER_BIT;
        glClearColor(data->clear.color[0], data->clear.color[1],
                     data->clear.color[2], data->clear.color[3]);
        glClear(mask);
}

void THE_CreateBufferExecute(THE_CommandData *data)
{
        leep::Renderer &r = leep::Manager::instance().renderer();
	leep::Buffer buff = data->createbuff.buffer;

        LEEP_CORE_ASSERT(r.buffers_[buff.handle()].cpu_version_ > 0, "This buffer hasn't got any data yet");

        glGenBuffers(1, &(r.buffers_[buff.handle()].internal_id_));

        if (buff.type() == leep::BufferType::INDEX_BUFFER)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r.buffers_[buff.handle()].internal_id_);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                        r.buffers_[buff.handle()].count_ * sizeof(uint32_t),
                        (const void*)r.buffers_[buff.handle()].data_.indices_,
                        GL_STATIC_DRAW);
        }
        else if (buff.type() != leep::BufferType::NONE)
        {
            glBindBuffer(GL_ARRAY_BUFFER, r.buffers_[buff.handle()].internal_id_);
            glBufferData(GL_ARRAY_BUFFER,
                        r.buffers_[buff.handle()].count_ * sizeof(float),
                        (const void*)r.buffers_[buff.handle()].data_.vertices_,
                        GL_STATIC_DRAW);
        }
        
        r.buffers_[buff.handle()].gpu_version_ = r.buffers_[buff.handle()].cpu_version_;

        // Delete ram data now that has been copied into the vram
        buff.freeSystemRamData();
}

#endif // THE_OPENGL

#endif
