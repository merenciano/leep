#ifndef THE_CORE_HMAP_H
#define THE_CORE_HMAP_H

#include <stdint.h>

#define THE_HMAP_INVALID_VALUE (void*)0xFFFFFFFFFFFFFFFF

typedef struct THE_HMap THE_HMap;

THE_HMap *THE_HMapCreate(uint32_t capacity, uint32_t value_size);
void THE_HMapInsert(THE_HMap *hm, const char *key, void *value);
void *THE_HMapGet(THE_HMap *hm, const char *key);
int32_t THE_HMapCount(THE_HMap *hm);
int32_t THE_HMapCapacity(THE_HMap *hm);
void THE_HMapClear(THE_HMap *hm);
void THE_HMapDelete(THE_HMap *hm);

#endif
