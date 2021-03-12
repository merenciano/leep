#include "buddy-alloc.h"

#include "core/common-defs.h"
#include "core/manager.h"
#include "core/memory/memory.h"
namespace leep {

CList::CList()
{

}

void CList::init()
{
    first_.prev_ = &first_;
    first_.next_ = &first_;
}

void CList::RemoveFromItsList(Node *n)
{
    Node *prev = n->prev_;
    Node *next = n->next_;
    prev->next_ = next;
    next->prev_ = prev;
}

void CList::push(Node *n)
{
    Node *prev = first_.prev_;
    n->prev_ = prev;
    n->next_ = &first_;
    prev->next_ = n;
    first_.prev_ = n;
}

CList::Node *CList::pop()
{
    Node *back = first_.prev_;
    if (back == &first_)
        return nullptr;
    CList::RemoveFromItsList(back);
    return back;
}

BuddyAlloc::BuddyAlloc() 
{
    mem_ = nullptr;
    mem_offset_ = nullptr;
    block_limit_ = 0;
}

void BuddyAlloc::init()
{
    mem_ = (int8_t*)GM.memory().persistentAlloc(kMaxAlloc);
    block_limit_ = kBlockCount - 1;
    updateOffset(mem_ + sizeof(CList::Node));
    blocks_[kBlockCount - 1].init();
    blocks_[kBlockCount - 1].push((CList::Node*)mem_);
}

inline void BuddyAlloc::updateOffset(int8_t *offset)
{
    LEEP_CORE_ASSERT(offset - mem_ <= kMaxAlloc, "Buddy out of space");
    if (offset > mem_offset_)
    {
        mem_offset_ = offset;
    }
}

int8_t *BuddyAlloc::getPtr(uint32_t index, uint32_t block)
{
    return mem_ + ((index - ( 1 << block) + 1) << (kMaxAllocExp - block));
}

uint32_t BuddyAlloc::getNode(int8_t *ptr, uint32_t block)
{
    return (uint32_t)((ptr - mem_) >> (kMaxAllocExp - block)) + (1<<block) - 1;
}

bool BuddyAlloc::parentSplit(uint32_t index)
{
    index = (index - 1) >> 1;
    return (split_info_[index / 8] >> (index % 8)) & 1;
}

void BuddyAlloc::flipParentSplit(uint32_t index)
{
    index = (index - 1) >> 1;
    split_info_[index / 8] ^= 1 << (index % 8);
}

uint32_t BuddyAlloc::adequateBlock(size_t request_size)
{
    uint32_t block = kBlockCount - 1;
    size_t size = kMinAlloc;

    while (size < request_size)
    {
        block--;
        size = size << 1;
    }

    return block;
}

void BuddyAlloc::lowerBlockLimit(uint32_t block)
{
    while (block < block_limit_)
    {
        uint32_t root = getNode(mem_, block_limit_);
        int8_t *right_child;

        if (!parentSplit(root))
        {
            CList::RemoveFromItsList((CList::Node*)mem_);
            blocks_[--block_limit_].init();
            blocks_[block_limit_].push((CList::Node*)mem_);
            continue;
        }

        right_child = getPtr(root + 1, block_limit_);
        updateOffset(right_child + sizeof(CList::Node));
        blocks_[block_limit_].push((CList::Node*)right_child);
        blocks_[--block_limit_].init();

        root = (root - 1) / 2;
        if (root != 0)
        {
            flipParentSplit(root);
        }
    }
}

void *BuddyAlloc::alloc(size_t size)
{
    uint32_t original_block;
    uint32_t block;

    LEEP_CORE_ASSERT(mem_ != nullptr, "You must init the buddy alloc");
    LEEP_CORE_ASSERT(size + kHeaderSize <= kMaxAlloc, "Buddy not big enough");

    block = adequateBlock(size + kHeaderSize);
    original_block = block;

    while (block + 1 != 0)
    {
        size_t rsize;
        size_t bytes_needed;
        uint32_t i;
        int8_t *ptr;

        lowerBlockLimit(block);

        ptr = (int8_t*)blocks_[block].pop();
        if (!ptr)
        {
            if (block != block_limit_ || block == 0)
            {
                block--;
                continue;
            }

            lowerBlockLimit(block - 1);
            ptr = (int8_t*)blocks_[block].pop();
        }

        rsize = (size_t)(1 << (kMaxAllocExp - block));
        if (block < original_block)
        {
            bytes_needed = rsize / 2 + sizeof(CList::Node);
        }
        else
        {
            bytes_needed = rsize;
        }
        mem_offset_ = ptr + bytes_needed; 

        i = getNode(ptr, block);
        if (i != 0)
        {
            flipParentSplit(i);
        }

        while (block < original_block)
        {
            i = i * 2 + 1;
            block++;
            flipParentSplit(i);
            blocks_[block].push((CList::Node*)getPtr(i + 1, block));

        }
        *(size_t*)ptr = size;
        return ptr + kHeaderSize;
    }
    return nullptr;
}


void BuddyAlloc::free(void *ptr)
{
    uint32_t block;
    uint32_t i;

    if (!ptr) return;

    ptr = (int8_t*)ptr - kHeaderSize;
    block = adequateBlock(*(uint32_t*)ptr + kHeaderSize);
    i = getNode((int8_t*)ptr, block);

    while (i != 0)
    {
        flipParentSplit(i);
        if (parentSplit(i) || block == block_limit_)
        {
            break;
        }

        CList::RemoveFromItsList((CList::Node*)getPtr(((i-1) ^ 1) + 1, block));
        i = (i - 1) / 2;
        block--;

    }
    // Add this address to the free list
    blocks_[block].push((CList::Node*)getPtr(i, block));
}

} // namespace leep
