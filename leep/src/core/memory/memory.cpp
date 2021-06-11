#include "memory.h"
#include "render/display-list.h"

#include <stdlib.h>

namespace leep {

Memory::Memory()
{
    mem_ = nullptr;
    offset_ = nullptr;
    capacity_ = 0;
}

Memory::~Memory()
{
    if (mem_)
    {
        free(mem_);
    }
    mem_ = nullptr;
    offset_ = nullptr;
}

void Memory::init(size_t size)
{
    mem_ = (int8_t*)malloc(size);
    if (!mem_)
    {
        LEEP_CORE_ERROR("Couldn't allocate Leep's memory");
        exit(1);
    }
    offset_ = mem_;
    capacity_ = size;
    buddy_.init();

}

void *Memory::persistentAlloc(size_t size)
{
    LEEP_CORE_ASSERT((size_t)(offset_ + size - mem_) <= capacity_, "Out of memory");
    if ((size_t)(offset_ + size - mem_) > capacity_)
    {
        LEEP_CORE_ERROR("Persistent allocation failed, Out of memory!");
        return nullptr;
    }
    void *mem = (void*)offset_;
    offset_ += size;
    return mem; 
}

void *Memory::generalAlloc(size_t size)
{
    return buddy_.alloc(size);
}

void *Memory::generalRealloc(void *ptr, size_t size)
{
    return buddy_.realloc(ptr, size);
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
    return bytesUsed() / (1024.0f * 1024.0f);
}

size_t Memory::bytesUsed() const
{
    return offset_ - mem_;
}

} // namespace leep
