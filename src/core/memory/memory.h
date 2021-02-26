// Lucas Merenciano Martinez  <lucasmermar@gmail.com>

#ifndef __LEEP_CORE_MEMORY_H__
#define __LEEP_CORE_MEMORY_H__

#include "core/memory/entity-container.h"

#include <unordered_map>

#define KILOBYTES(X) ((X) * 1024)
#define MEGABYTES(X) (KILOBYTES(X) * 1024)
#define GIGABYTES(X) (MEGABYTES(X) * 1024)

namespace leep
{
    // TODO: Draw command with pointer to material will reduce vastly
    // the memory stored in the pool (due to materialdata union being big)
    const uint32_t kRenderPoolSize = KILOBYTES(100);
    const uint32_t kRenderQueueMaxCount = 500;


    class Memory
    {
        // TODO: handle heap allocator (int allocator)
        //       per frame allocator?
        //       general alloc (region based?, buddy?)
    public:
        Memory();
        void init();
        void freeAll();
        void *alloc(int32_t size);
        float mbUsed() const;
        size_t bytesUsed() const;

        // Entities
        void createContainer(EntityType t);
        EntityContainer &container(EntityType t);
        std::unordered_map<EntityType, EntityContainer> entities_;

        int8_t *mem_;
        int8_t *offset_;
    };
}

#endif // __LEEP_CORE_MEMORY_H__