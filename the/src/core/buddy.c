#include "buddy.h"
#include "mem.h"

#define BUDDY_ALLOC_IMPLEMENTATION
#include "buddy_alloc.h"  //https://github.com/spaskalev/buddy_alloc

static u8 *buddy_meta = NULL;
static u8 *buddy_chunk = NULL;
static struct buddy *buddy = NULL;

void THE_BuddyInit(size_t size)
{
	THE_ASSERT(!buddy_meta && !buddy_chunk && !buddy, "Buddy allocator already initialized");
	buddy_meta = (u8*)THE_PersistentAlloc(buddy_sizeof(size), 16);
	buddy_chunk = (u8*)THE_PersistentAlloc(size, 16);
	buddy = buddy_init(buddy_meta, buddy_chunk, size);
}

void *THE_BuddyAlloc(size_t size)
{
	return buddy_malloc(buddy, size);
	//return malloc(size);
}

void *THE_BuddyCalloc(size_t element_count, size_t element_size)
{
	return buddy_calloc(buddy, element_count, element_size);
	//return calloc(element_count, element_size);
}

void *THE_BuddyRealloc(void *ptr, size_t size)
{
	return buddy_realloc(buddy, ptr, size);
	//return realloc(ptr, size);
}

void THE_BuddyFree(void *data)
{
	buddy_free(buddy, data);
	//free(data);
}

