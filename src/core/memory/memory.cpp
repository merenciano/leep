#include "memory.h"
#include "render/display-list.h"

#include <stdlib.h>

namespace leep {

Memory::Memory()
{
    mem_ = nullptr;
    offset_ = nullptr;
}

void Memory::init()
{
    mem_ = (int8_t*)malloc(kTotalMemSize);
    if (!mem_)
    {
        LEEP_CORE_ERROR("Couldn't allocate Leep's memory");
        exit(1);
    }
    offset_ = mem_;
    buddy_.init();
}

void *Memory::persistentAlloc(size_t size)
{
    LEEP_CORE_ASSERT(offset_ + size - mem_ < kTotalMemSize, "Out of memory");
    void *mem = (void*)offset_;
    offset_ += size;
    return mem; 
}

void *Memory::generalAlloc(size_t size)
{
    return buddy_.alloc(size);
}

void Memory::generalFree(void *ptr)
{
    buddy_.free(ptr);
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

} // namespace leep