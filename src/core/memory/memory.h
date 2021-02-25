// Lucas Merenciano Martinez  <lucasmermar@gmail.com>

#ifndef __LEEP_CORE_MEMORY_H__
#define __LEEP_CORE_MEMORY_H__ 1

#include "core/memory/entity-container.h"

#include <unordered_map>

namespace leep
{
    const uint32_t kRenderQueueSize = 10 * 1024;
    class RenderQAlloc
    {
    public:
        void init(int32_t chunk_size)
        {
            curr_renderq_ = (uint8_t*)malloc(chunk_size);
            next_renderq_ = (uint8_t*)malloc(chunk_size);
            next_offset_ = next_renderq_;
        }

        void* alloc(int32_t size)
        {
            void* mem = (void*)next_offset_;
            next_offset_ += size;
            return mem;
        }

        template<typename T>
        T* allocT()
        {
            T* mem = new(next_offset_) T();
            next_offset_ += sizeof(T);
            LEEP_CORE_ASSERT(next_offset_ - next_renderq_ < kRenderQueueSize, "Queue doesn't fit in the chunk");
            return mem;
        }

        void swapQueues()
        {
            uint8_t* tmp = curr_renderq_;
            curr_renderq_ = next_renderq_;
            next_renderq_ = tmp;
            next_offset_ = next_renderq_;
        }

        uint8_t* curr_renderq_; /// Current rendering queue (Main thread renders this)
        uint8_t* next_renderq_; /// Next rendering queue (Logic thread fill this for the next frame)
        uint8_t* next_offset_;
    };

    class Memory
    {
        
    public:
        Memory();

        EntityContainer& container(EntityType t);
        void createContainer(EntityType t);
    //private:
        std::unordered_map<EntityType, EntityContainer> entities_;

        RenderQAlloc renderq_;
    };

    
}

#endif // __LEEP_CORE_MEMORY_H__