#ifndef __THE_RENDER_INTERNAL_RESOURCES_H__
#define __THE_RENDER_INTERNAL_RESOURCES_H__

#include "core/Cdefinitions.h"
#include "Crenderer.h" // TODO: Solo por THE_BufferType y THE_TexType... revisar

typedef u32 THE_InternalMaterial; // OpenGL program (compiled shaders)

typedef struct {
	union {
		float *vertices;
		u32 *indices;
	};
	
	s32 count;
	u32 internal_id;
	s32 cpu_version;
	s32 gpu_version;
	THE_BufferType type;
} THE_InternalBuffer;

typedef struct {
        char path[64];
        void *pix;
        s32 internal_id;
        s32 cpu_version;
        s32 gpu_version;
        u32 texture_unit;
        u32 width;
        u32 height;
        THE_TexType type;
} THE_InternalTexture;

typedef struct {
	s32 internal_id;
	s32 cpu_version;
	s32 gpu_version;
	u32 width;
	u32 height;
	THE_Texture color_tex;
	THE_Texture depth_tex;
	u8 color;
	u8 depth;
} THE_InternalFramebuffer;

extern THE_InternalBuffer *buffers;
extern THE_InternalTexture *textures;
extern THE_InternalFramebuffer *framebuffers;
extern THE_InternalMaterial* materials;
extern u16 buffer_count;
extern u16 texture_count;
extern u16 framebuffer_count;

s32 IsValidBuffer(THE_Buffer buff);
s32 IsValidTexture(THE_Texture tex);
s32 IsValidFramebuffer(THE_Framebuffer fb);

u32 InitInternalMaterial(const char* shader_name);
void UseMaterial(THE_Material* mat);

#endif
