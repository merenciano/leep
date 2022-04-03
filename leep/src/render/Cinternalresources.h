#ifndef __THE_RENDER_INTERNAL_RESOURCES_H__
#define __THE_RENDER_INTERNAL_RESOURCES_H__

#include "core/Cdefinitions.h"

typedef enum {
	THE_BUFFER_NONE = 0,
	THE_BUFFER_VERTEX_3P,
	THE_BUFFER_VERTEX_3P_3N,
	THE_BUFFER_VERTEX_3P_2UV,
	THE_BUFFER_VERTEX_3P_3N_2UV,
	THE_BUFFER_VERTEX_3P_3N_3T_3B_2UV,
	THE_BUFFER_INDEX,
} THE_BufferType;

typedef enum {
	THE_TEX_NONE = 0,
	THE_TEX_R,
	THE_TEX_RGB,
	THE_TEX_SRGB,
	THE_TEX_DEPTH,
	THE_TEX_SKYBOX,
	THE_TEX_RGB_F16,
	THE_TEX_RGBA_F16,
	THE_TEX_LUT,
	THE_TEX_ENVIRONMENT,
	THE_TEX_PREFILTER_ENVIRONMENT,
} THE_TexType;

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
        s8 path_[64];
        void *pix_;
        s32 internal_id_;
        s32 cpu_version_;
        s32 gpu_version_;
        u32 texture_unit_;
        u32 width_;
        u32 height_;
        THE_TexType type_;
} THE_InternalTexture;

#endif
