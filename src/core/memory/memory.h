// Lucas Merenciano Martinez  <lucasmermar@gmail.com>

#ifndef __LEEP_CORE_MEMORY_H__
#define __LEEP_CORE_MEMORY_H__

#include "core/memory/entity-container.h"
#include "core/memory/buddy-alloc.h"

#include <unordered_map>

#define KILOBYTES(X) ((X) * 1024)
#define MEGABYTES(X) (KILOBYTES(X) * 1024)
#define GIGABYTES(X) (MEGABYTES(X) * 1024)

namespace leep
{
    const uint64_t kTotalMemSize = GIGABYTES((uint64_t)2);

    class Memory
    {
        // TODO: handle heap allocator (int allocator)
        //       per frame allocator?
        //       general alloc (region based?, buddy?)
    public:
        Memory();
        void init();
        void freeAll();
        void *persistentAlloc(size_t size);
        // General alloc wrappers
        inline void *generalAlloc(size_t size);
        template<typename T> inline T *generalAllocT(size_t count);
        inline void generalFree(void *ptr);

        float mbUsed() const;
        size_t bytesUsed() const;

        // Entities
        void createContainer(EntityType t);
        EntityContainer &container(EntityType t);
        std::unordered_map<EntityType, EntityContainer> entities_;

        // TODO: look for a better way to access this data from
        // the memory inspector tool and make private the variables below
        int8_t *mem_;
        int8_t *offset_;
        BuddyAlloc buddy_;
    };

    template<typename T>
    T *Memory::generalAllocT(size_t count)
    {
        return buddy_.allocT<T>(count);
    }
}

#endif // __LEEP_CORE_MEMORY_H__
