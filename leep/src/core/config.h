/* Lucas Merenciano Martinez  <lucasmermar@gmail.com> */

#ifndef THE_CORE_CONFIG_H
#define THE_CORE_CONFIG_H

#include "core/Cdefinitions.h"

const char kWindowTitle[] = "Leep";
const s32 kMaxTextures = 63;
const s32 kMaxBuffers = 128;

typedef struct THE_Config 
{
    // Memory chunk size for the buddy allocator (in bytes)
    size_t alloc_capacity = GIGABYTES((u64)1);
    // Maximum renderable entities in the scene
    // This is for engine allocation purposes
    s32 render_queue_capacity = 12000;
    s32 max_geometries = 128;
    s32 window_width = 1280;
    s32 window_height = 720;
    bool vsync = true;
} THE_Config;

typedef struct THE_GameData
{
    float delta_time_;
} THE_GameData;

#endif
