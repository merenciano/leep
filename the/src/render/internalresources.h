#ifndef __THE_RENDER_INTERNAL_RESOURCES_H__
#define __THE_RENDER_INTERNAL_RESOURCES_H__

#include "core/definitions.h"
#include "renderer.h" // TODO: Solo por THE_BufferType y THE_TexType... revisar

typedef uint32_t THE_InternalMaterial; // OpenGL program (compiled shaders)

typedef struct {
	union {
		float *vertices;
		uint32_t *indices;
	};
	int32_t count;
	uint32_t internal_id;
	int32_t cpu_version;
	int32_t gpu_version;
	THE_BufferType type;
} THE_InternalBuffer;

typedef struct {
	char path[64];
	void *pix;
	int32_t internal_id;
	int32_t cpu_version;
	int32_t gpu_version;
	uint32_t texture_unit;
	uint32_t width;
	uint32_t height;
	THE_TexType type;
} THE_InternalTexture;

typedef struct {
	int32_t internal_id;
	int32_t cpu_version;
	int32_t gpu_version;
	uint32_t width;
	uint32_t height;
	THE_Texture color_tex;
	THE_Texture depth_tex;
	uint8_t color;
	uint8_t depth;
} THE_InternalFramebuffer;

extern THE_InternalBuffer *buffers;
extern THE_InternalTexture *textures;
extern THE_InternalFramebuffer *framebuffers;
extern THE_InternalMaterial *materials;
extern uint16_t buffer_count;
extern uint16_t texture_count;
extern uint16_t framebuffer_count;

int32_t IsValidBuffer(THE_Buffer buff);
int32_t IsValidTexture(THE_Texture tex);
int32_t IsValidFramebuffer(THE_Framebuffer fb);

uint32_t InitInternalMaterial(const char *shader_name);
void UseMaterial(THE_Material *mat);

#endif
