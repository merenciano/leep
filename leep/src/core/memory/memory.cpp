#include "memory.h"

#include "core/Cdefinitions.h"
//#include "Cbuddy.h"

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
        THE_SLOG_ERROR("Couldn't allocate Leep's memory");
        exit(1);
    }
    offset_ = mem_;
    capacity_ = size;
    //buddy_.init();
	THE_BuddyInit(1U << 29); // 1GB
}

void *Memory::persistentAlloc(size_t size)
{
    THE_ASSERT((size_t)(offset_ + size - mem_) <= capacity_, "Out of memory");
    if ((size_t)(offset_ + size - mem_) > capacity_)
    {
	    THE_SLOG_ERROR("Persistent allocation failed, Out of memory!");
        return nullptr;
    }
    void *mem = (void*)offset_;
    offset_ += size;
    return mem; 
}

void *Memory::generalAlloc(size_t size)
{
	return THE_BuddyAlloc(size);
    //return buddy_.alloc(size);
}

void *Memory::generalCalloc(size_t element_count, size_t element_size)
{
	return THE_BuddyCalloc(element_count, element_size);
}

void *Memory::generalRealloc(void *ptr, size_t size)
{
	return THE_BuddyRealloc(ptr, size);
    //return buddy_.realloc(ptr, size);
}

void Memory::generalFree(void *ptr)
{
	THE_BuddyFree(ptr);
    //buddy_.free(ptr);
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
