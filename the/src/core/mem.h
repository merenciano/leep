#ifndef THE_CORE_MEM_H
#define THE_CORE_MEM_H

#include "definitions.h"

void *THE_PersistentAlloc(size_t size, size_t align);
void *THE_Alloc(size_t size);
void *THE_Calloc(s32 elem_count, size_t elem_size);
void *THE_Realloc(void *ptr, size_t size);
void THE_Free(void *ptr);

void THE_MemInit(size_t size);
void THE_MemFreeAll();
float THE_MemUsedMB();
size_t THE_MemUsedBytes();
size_t THE_MemCapacity();

#endif
