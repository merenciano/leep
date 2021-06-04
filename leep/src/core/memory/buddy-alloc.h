#ifndef __LEEP_CORE_MEMORY_BUDDY_ALLOCATOR_H__
#define __LEEP_CORE_MEMORY_BUDDY_ALLOCATOR_H__

// Needed for size_t on linux...
#include <stddef.h>
#include <stdint.h>
#include <mutex>

namespace leep
{
    const uint32_t kHeaderSize = 8;
    const uint32_t kMinAllocExp = 4;
    const uint32_t kMaxAllocExp = 30;
    const size_t kMinAlloc = 1 << kMinAllocExp;
    const size_t kMaxAlloc = 1 << kMaxAllocExp;
    const uint32_t kBlockCount = kMaxAllocExp - kMinAllocExp + 1;

    struct CList
    {
        struct Node
        {
            Node *prev_;
            Node *next_;
        };
        static void RemoveFromItsList(Node *n);
        CList();
        ~CList() {};
        void init();
        void push(Node *n);
        Node *pop();
        Node first_;
    };

    class BuddyAlloc
    {
    public:
        BuddyAlloc();
        void *alloc(size_t size);
        void *realloc(void *p, size_t new_size);
        template<typename T> T *allocT(size_t count = 1);
        void free(void *ptr);
        void init();

        size_t mem_used_;

    private:
        inline void updateOffset(int8_t *offset);
        int8_t *getPtr(uint32_t index, uint32_t block);
        uint32_t getNode(int8_t *ptr, uint32_t block);
        bool parentSplit(uint32_t index);
        void flipParentSplit(uint32_t index);
        uint32_t adequateBlock(size_t request_size);
        size_t blockSize(uint32_t block);
        void lowerBlockLimit(uint32_t block);

        // One free list for each block size (from 16B to 2GB)
        CList blocks_[kBlockCount];
        uint32_t block_limit_;
        uint8_t split_info_[(1 << (kBlockCount - 1)) / 8];

        int8_t *mem_;
        int8_t *mem_offset_;
        std::mutex mtx_;
    };

    template<typename T>
    T *BuddyAlloc::allocT(size_t count)
    {
        T* mem = (T*)alloc(sizeof(T) * count);
        for (size_t i = 0; i < count; ++i)
        {
            new(mem + i) T();
        }
        return mem;
    }
}

#endif  // __LEEP_CORE_MEMORY_BUDDY_ALLOCATOR_H__
