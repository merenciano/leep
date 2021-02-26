#include "memory.h"
#include "render/display-list.h"

namespace leep {

Memory::Memory()
{
    mem_ = nullptr;
    offset_ = nullptr;
}

void Memory::init()
{
    mem_ = (int8_t*)malloc(GIGABYTES((uint64_t)2));
    offset_ = mem_;
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

} // namespace leep