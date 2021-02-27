#ifndef __LEEP_CORE_MEMORY_BUDDY_ALLOCATOR_H__
#define __LEEP_CORE_MEMORY_BUDDY_ALLOCATOR_H__


namespace leep
{
    struct CList
    {
        static void RemoveFromItsList(Node *n)
        {
            Node *prev = n->prev_;
            Node *next = n->next_;
            prev->next_ = next;
            next->prev_ = prev;
        }

        struct Node
        {
            CList *prev_;
            CList *next_;
        };

        void init()
        {
            first_->prev_ = first_;
            first_->next_ = first_;
        }

        void push(Node *n)
        {
            Node *prev = first_->prev_;
            n->prev_ = prev;
            n->next_ = first_;
            prev->next_ = n;
            first_->prev_ = n;
        }

        Node *pop()
        {
            Node *back = first_->prev_;
            if (back == first_)
                return nullptr;
            CList::RemoveFromItsList(back);
            return back;
        }

        Node *first_;
    };

    class BuddyAlloc
    {
        const int32_t kHeaderSize = 8;
        const int32_t kMinAllocExp = 4;
        const int32_t kMaxAllocExp = 31;
        constexpr int32_t kMinAlloc = 1 << kMinAllocExp; // 8 of header + 8
        constexpr int32_t kMaxAlloc = 1 << kMaxAllocExp;
        const int32_t kRowCount = kMaxAllocExp - kMinAllocExp + 1;

    public:
        void init();
        int8_t *getPointer(int32_t index, int32_t row)
        {
            return mem_ + ((index - ( 1 << row) + 1) << (kMaxAllocExp - row));
        }
        
        int32_t getNode(int8_t *ptr, int32_t row)
        {
            return ((ptr - mem_) >> (kMaxAllocExp - row)) + (1<<row) - 1;
        }

        bool parentSplit(int32_t index)
        {
            index = (index - 1) >> 1;
            return (split_info_[index >> 3] >> (index & 7)) & 1;
        }

        void flipParentSplit(int32_t index)
        {
            index = (index - 1) >> 1;
            split_info_[index >> 3] ^= 1 << (index & 7);
        }

        int32_t bestFit(int32_t request)
        {
            int32_t row = kRowCount - 1;
            int32_t size = kMinAlloc;

            while (size < request)
            {
                --row;
                size = size << 1;
            }

            return row;
        }

        void lowerRowLimit(int32_t row)
        {
            while (row < row_limit_)
            {
                int32_t root = getNode(mem_, row_limit_);
                int32_t *right_child;

                if (!parentSplit(root))
                {
                    CList::RemoveFromItsList((CList::Node*)mem_);
                    rows_[--row_limit].init();
                    rows_[row_limit].push((CList::Node*)mem_);
                    continue;
                }

                right_child = getPtr(root + 1, row_limit);
                max_ptr_ = right_child + sizeof(GList::Node);
                rows_[row_limit].push((GList::Node*)right_child);
                rows_[--row_limit].init();

                root = (root - 1) / 2;
                if (root != 0)
                {
                    flipParentSplit(root);
                }
            }
        }

        void *alloc(int32_t size)
        {
            int32_t original_row;
            int32_t row;

            if (size + kHeaderSize > kMaxAlloc)
            {
                return nullptr;
            }

            row = bestFit(size + kHeaderSize);
            original_row = row;

            while (row + 1 != 0)
            {
                int32_t rsize;
                int32_t bytes_needed;
                int32_t i;
                int8_t *ptr;

                lowerRowLimit(row);

                ptr = (int8_t*)rows_[row].pop();
                if (!ptr)
                {
                    if (row != row_limit || row == 0)
                    {
                        row--;
                        continue;
                    }

                    lowerRowLimit(row - 1);
                    ptr = (int8_t*)rows_[row].pop();
                }

                rsize = 1 << (kMaxAllocExp - row);
                bytes_needed = row < original_row ? rsize / 2 + sizeof(CList::Node) : rsize;
                max_offset_ = ptr + bytes_needed; 

                i = getNode(ptr, row);
                if (i != 0)
                {
                    flipParentSplit(i);
                }

                while (row < original_row)
                {
                    i = i * 2 + 1;
                    row++;
                    flipParentSplit(i);
                    rows_[row].push((CList::Node*)getPtr(i + 1, row));

                    *(int32_t*)ptr = size;
                    return ptr + HEADER_SIZE;
                }
                return nullptr;
            }



        }
        // TODO: contructor with the mem_ alloc and first list init (see base_ptr == NULL) 

        CList rows_[ROW_COUNT];
        int32_t row_limit_;
        uint8_t split_info_[(1 << (ROW_COUNT - 1)) >> 3];
        
        uint8_t *mem_;
        uint8_t *max_offset_;
    };
}

#endif  // __LEEP_CORE_MEMORY_BUDDY_ALLOCATOR_H__

