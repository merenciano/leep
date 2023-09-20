#include "mem.h"
#include "buddy.h"
#include <stdlib.h>

static uint8_t *mem;
static uint8_t *offset;
static size_t capacity;

void *THE_PersistentAlloc(size_t size, size_t align)
{
    THE_ASSERT((size_t)(offset + size - mem) <= capacity, "Out of memory");
    if ((size_t)(offset + size - mem) > capacity) {
	    THE_SLOG_ERROR("Persistent allocation failed, Out of memory!");
        return NULL;
    }

    void *ret_mem = (void*)offset;
    offset += size;
	if (align != 0 && (size_t)offset % align) //TODO: think about a fancier align method
	{
		offset += (align - ((size_t)offset % align));
	}
    return ret_mem; 
}

void *THE_Alloc(size_t size)
{
	return THE_BuddyAlloc(size);
}

void *THE_Calloc(s32 elem_count, size_t elem_size)
{
	return THE_BuddyCalloc(elem_count, elem_size);
}

void *THE_Realloc(void *ptr, size_t size)
{
	return THE_BuddyRealloc(ptr, size);
}

void THE_Free(void *ptr)
{
	THE_BuddyFree(ptr);
}

void THE_MemInit(size_t size)
{
	mem = (u8*)malloc(size);
	if (!mem) {
		THE_SLOG_ERROR("Couldn't allocate THE memory.");
		exit(1);
	}

	offset = mem;
	capacity = size;
	THE_BuddyInit(1U << 29); // 1GB
}

void THE_MemFreeAll()
{
	free(mem);
	mem = NULL;
}

float THE_MemUsedMB()
{
	return THE_MemUsedBytes() / (1024.0f * 1024.0f);
}

size_t THE_MemUsedBytes()
{
	return offset - mem;
}

size_t THE_MemCapacity()
{
	return capacity;
}
