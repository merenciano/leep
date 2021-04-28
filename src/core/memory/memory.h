// Lucas Merenciano Martinez  <lucasmermar@gmail.com>

#ifndef __LEEP_CORE_MEMORY_H__
#define __LEEP_CORE_MEMORY_H__

#include "core/memory/buddy-alloc.h"

#define KILOBYTES(X) ((X) * 1024)
#define MEGABYTES(X) (KILOBYTES(X) * 1024)
#define GIGABYTES(X) (MEGABYTES(X) * 1024)

namespace leep
{
    const uint64_t kTotalMemSize = MEGABYTES((uint64_t)1224);

    class Memory
    {
    public:
        Memory();
        ~Memory();
        void init();
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
    };

    template<typename T>
    T *Memory::generalAllocT(size_t count)
    {
        return buddy_.allocT<T>(count);
    }
}

#endif // __LEEP_CORE_MEMORY_H__
