// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_CORE_MEMORY_STL_BUDDY_H__
#define __LEEP_CORE_MEMORY_STL_BUDDY_H__

#include "core/manager.h"

namespace leep
{
    template <typename T>
    class stl_buddy
    {
    public:
        typedef size_t size_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T value_type;

        stl_buddy(){}
        ~stl_buddy(){}

        template <class U> struct rebind { typedef stl_buddy<U> other; };
        template <class U> stl_buddy(const stl_buddy<U>&){}

        pointer address(reference x) const {return &x;}
        const_pointer address(const_reference x) const {return &x;}
        size_type max_size() const throw() {return size_t(-1) / sizeof(value_type);}

        pointer allocate(size_type n)
        {
            return static_cast<T*>(GM.memory().generalAlloc(n * sizeof(T)));
        }
        void deallocate(pointer p, size_type n)
        {
            GM.memory().generalFree(p);
        }

        void construct(pointer p, const T& val)
        {
            new(static_cast<void*>(p)) T(val);
        }

        void construct(pointer p)
        {
            new(static_cast<void*>(p)) T();
        }

        void destroy(pointer p)
        {
            p->~T();
        }
    };
} // namespace leep
#endif // __LEEP_CORE_MEMORY_STL_BUDDY_H__
