#include "hmap.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#define HMapGetHeader(HM) ((HMapHeader*)(HM) - 1)
#define HMapGetCapacity(HM) (HMapGetHeader(HM)->capacity)
#define HMapGetCount(HM) (HMapGetHeader(HM)->count)
#define HMapGetNodeSize(HM) (HMapGetHeader(HM)->node_size)
#define HMapGetValueSize(HM) (HMapGetHeader(HM)->value_size)

typedef struct HMapHeader {
	uint32_t capacity;
	uint32_t count;
	uint32_t node_size;
	uint32_t value_size;
} HMapHeader;

typedef struct HMapKey {
	union {
		char name[8];
		uint64_t hash;
	};
} HMapKey;

typedef struct THE_HMap {
	HMapKey key;
	void *value;
} HMap;

typedef HMap THE_HMap;

static inline uint32_t NextPow2(uint32_t v)
{
	--v;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	return v + 1;
}

static inline uint64_t HMapHash(const char *key)
{
	HMapKey mk;
	mk.hash = 0;
	strncpy(mk.name, key, 8);
	return mk.hash;
}

static inline HMap *GetNode(HMap *hm, uint32_t offset)
{
	return (HMap*)((uint8_t*)hm + HMapGetNodeSize(hm) * offset);
}

THE_HMap *THE_HMapCreate(uint32_t capacity, uint32_t value_size)
{
	assert(capacity <= (1 << 31));
	capacity = NextPow2(capacity);
	size_t map_size = (value_size + sizeof(HMapKey)) * capacity + sizeof(HMapHeader);
	HMapHeader *hm = (HMapHeader*)calloc(map_size, 1);

	hm->capacity = capacity;
	hm->count = 0;
	hm->node_size = value_size + sizeof(HMapKey);
	hm->value_size = value_size;

	return (HMap*)(hm + 1);
}

void THE_HMapDelete(HMap *hm)
{
	free((HMapHeader*)hm - 1);
}

void THE_HMapInsert(HMap *hm, const char *key, void *value)
{
	assert(value);
	assert(HMapGetCount(hm) < HMapGetCapacity(hm)); // Change this to 80% capacity

	HMapKey mk;
	mk.hash = HMapHash(key);
	printf("Key %s = %s : Hash: 0x%lX\n", key, mk.name, mk.hash);
	uint32_t offset = mk.hash & (HMapGetCapacity(hm) - 1);

	HMap *node;
	for (node = GetNode(hm, offset); node->key.hash != 0; node = GetNode(hm, offset)) {
		if (node->key.hash == mk.hash) {
			memcpy(&(node->value), value, (HMapGetValueSize(hm)));
			printf("Key %s already here!\n", key);
			return;
		}
		//printf("Collision\n");
		offset = (offset + 1) & (HMapGetCapacity(hm) - 1);
	}

	//printf("Inserted\n");
	node->key.hash = mk.hash;
	memcpy(&(node->value), value, (HMapGetValueSize(hm)));
	++HMapGetCount(hm);
}

void *THE_HMapGet(HMap *hm, const char *key)
{
	HMapKey mk;
	mk.hash = HMapHash(key);
	uint32_t offset = mk.hash & (HMapGetCapacity(hm) - 1);

	HMap *node;
	for (node = GetNode(hm, offset); node->key.hash != mk.hash; node = GetNode(hm, offset)) {
		if (node->key.hash == 0) {
			return THE_HMAP_INVALID_VALUE;
		}
		offset = (offset + 1) & (HMapGetCapacity(hm) - 1);
	}

	return (void*)(&(node->value));
}

void THE_HMapClear(HMap *hm)
{
	memset(hm, 0, HMapGetCapacity(hm) * HMapGetNodeSize(hm));
	HMapGetCount(hm) = 0;
}

int32_t THE_HMapCount(HMap *hm)
{
	return HMapGetCount(hm);
}

int32_t THE_HMapCapacity(HMap *hm)
{
	return HMapGetCapacity(hm);
}