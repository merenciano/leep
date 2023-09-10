#ifndef THE_BUDDY_H
#define THE_BUDDY_H

#include "core/definitions.h"

void THE_BuddyInit(size_t size);
void *THE_BuddyAlloc(size_t size);
void *THE_BuddyCalloc(size_t element_count, size_t element_size);
void *THE_BuddyRealloc(void *ptr, size_t size);
void THE_BuddyFree(void *data);

#endif
