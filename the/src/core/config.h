#ifndef THE_CORE_CONFIG_H
#define THE_CORE_CONFIG_H

#include "core/definitions.h"

extern const char *WINDOW_TITLE;
extern const s32 MAX_TEXTURES;
extern const s32 MAX_BUFFERS;

typedef struct THE_Config {
	VoidFunc init_func;
	VoidFunc logic_func;
	VoidFunc close_func;
	size_t alloc_capacity; // Memory chunk size for the buddy allocator (in bytes).
	s32 render_queue_capacity; // Maximum renderable entities in the scene. This is for engine allocation purposes.
	s32 max_geometries;
	s32 window_width;
	s32 window_height;
	bool vsync;
} THE_Config;

#endif
