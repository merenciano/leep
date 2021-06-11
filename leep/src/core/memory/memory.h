// Lucas Merenciano Martinez  <lucasmermar@gmail.com>

#ifndef __LEEP_CORE_MEMORY_H__
#define __LEEP_CORE_MEMORY_H__

#include "core/common-defs.h"
#include "core/memory/buddy-alloc.h"

namespace leep
{
    class Memory
    {
    public:
        Memory();
        ~Memory();
        void init(size_t size);
        void freeAll();
        void *persistentAlloc(size_t size);
        // General alloc wrappers
        void *generalAlloc(size_t size);
        template<typename T> inline T *generalAllocT(size_t count);
        void *generalRealloc(void *ptr, size_t size);
        void generalFree(void *ptr);

        float mbUsed() const;
        size_t bytesUsed() const;

        int8_t *mem_;
        int8_t *offset_;
        BuddyAlloc buddy_;

        // TODO: Capacity should be private and have a getter only
        size_t capacity_;
    };

    template<typename T>
    T *Memory::generalAllocT(size_t count)
    {
        return buddy_.allocT<T>(count);
    }
}

#endif // __LEEP_CORE_MEMORY_H__
