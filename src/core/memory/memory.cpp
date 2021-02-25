#include "memory.h"
#include "render/display-list.h"

namespace leep
{
    Memory::Memory()
    {
        mem_ = nullptr;
        offset_ = nullptr;
    }

    void Memory::init()
    {
        mem_ = (int8_t*)malloc(GIGABYTES((uint64_t)2));
        offset_ = mem_;
        renderq_.init(this);
    }

    void *Memory::alloc(int32_t size)
    {
        void *mem = (void*)offset_;
        offset_ += size;
        return mem; 
    }

    void Memory::freeAll()
    {
        free(mem_);
    }

    float Memory::mbUsed() const
    {
        return (offset_ - mem_) / (1024.0f * 1024.0f);
    }

    size_t Memory::bytesUsed() const
    {
        return offset_ - mem_;
    }

    EntityContainer &Memory::container(EntityType t)
    {
        return entities_.at(t);
    }

    void Memory::createContainer(EntityType t)
    {
        entities_.emplace(std::make_pair(t, t));
    }

    void RenderQueues::init(Memory *m)
    {
        LEEP_CORE_ASSERT(curr_pool_ == nullptr,
            "Already initialized.");
        curr_pool_ = (int8_t*)m->alloc(kRenderPoolSize);
        next_pool_ = (int8_t*)m->alloc(kRenderPoolSize);
        next_offset_ = next_pool_;

        curr_queue_ = (DLComm**)m->alloc(kRenderQueueMaxCount * sizeof(int*));
        next_queue_ = (DLComm**)m->alloc(kRenderQueueMaxCount * sizeof(int*));
        curr_count_ = 0;
        next_count_ = 0;
    }

    void RenderQueues::addDL(DisplayList *dl)
    {
        LEEP_CORE_ASSERT(next_count_ + dl->commandListCount() < 
            kRenderQueueMaxCount, "Max queue count reached");
        for (int32_t i = 0; i < dl->commandListCount(); ++i)
        {
            next_queue_[next_count_] = &(dl->command_list())[i];
            printf("%p, %p\n", (void*)(next_queue_ + next_count_), (void*)(dl->command_list() + i));
            printf("%p, %p\n", &(next_queue_[next_count_]), &(dl->command_list()[i]));
            ++next_count_;
        }
    }
}