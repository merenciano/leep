// Lucas Merenciano Martinez  <lucasmermar@gmail.com>

#ifndef __LEEP_CORE_MEMORY_H__
#define __LEEP_CORE_MEMORY_H__ 1

#include "core/memory/entity-container.h"

#include <unordered_map>

#define KILOBYTES(X) ((X) * 1024)
#define MEGABYTES(X) (KILOBYTES(X) * 1024)
#define GIGABYTES(X) (MEGABYTES(X) * 1024)

namespace leep
{
    class Memory;
    class DisplayList;
    class DisplayListCommand;
    typedef DisplayListCommand DLComm;
    // TODO: Draw command with pointer to material will reduce vastly
    // the memory stored in the pool (due to materialdata union being big)
    const uint32_t kRenderPoolSize = 100 * 1024;
    const uint32_t kRenderQueueMaxCount = 500;

    class RenderQueues
    {
    public:
        RenderQueues()
        {
            curr_pool_ = nullptr;
            next_pool_ = nullptr;
            next_offset_ = nullptr;
            curr_queue_ = nullptr;
            next_queue_ = nullptr;
            curr_count_ = 0;
            next_count_ = 0;
        }

        void init(Memory *m);
        void addDL(DisplayList *dl);

        template<typename T>
        DLComm *commandAlloc()
        {
            DLComm *mem = new(next_offset_) T();
            next_offset_ += sizeof(T);
            LEEP_CORE_ASSERT(next_offset_ - next_pool_ < kRenderPoolSize,
                "Queue doesn't fit in the chunk");
            return mem;
        }


        void swapQueues()
        {
            int8_t *tmp = curr_pool_;
            curr_pool_ = next_pool_;
            next_pool_ = tmp;
            next_offset_ = next_pool_;

            DLComm **tmp2 = curr_queue_;
            curr_queue_ = next_queue_;
            curr_count_ = next_count_;
            next_queue_ = tmp2;
            next_count_ = 0;
        }

        int8_t *curr_pool_; // Main thread renders this
        int8_t *next_pool_; // Logic thread fill this for the next frame
        int8_t *next_offset_;
        DLComm **curr_queue_;
        DLComm **next_queue_;
        int16_t  curr_count_;
        int16_t  next_count_;
    };

    class Memory
    {
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

        RenderQueues renderq_;
        int8_t *mem_;
        int8_t *offset_;
    };
}

#endif // __LEEP_CORE_MEMORY_H__