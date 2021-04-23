// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_CORE_MEMORY_STL_BUDDY_H__
#define __LEEP_CORE_MEMORY_STL_BUDDY_H__

#include "core/manager.h"

namespace leep
{
    template <typename T>
    class BuddySTL
    {
    public:
        typedef size_t size_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T value_type;

        BuddySTL(){}
        ~BuddySTL(){}

        template <class U> struct rebind { typedef BuddySTL<U> other; };
        template <class U> BuddySTL(const BuddySTL<U>&){}

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

    template<class T, class U>
    inline bool operator==(const BuddySTL<T> &a, const BuddySTL<U> &b)
    {
        return true; // I only have one
    }

    template<class T, class U>
    inline bool operator!=(const BuddySTL<T> &a, const BuddySTL<U> &b)
    {
        return false; // I only have one
    }
} // namespace leep
#endif // __LEEP_CORE_MEMORY_STL_BUDDY_H__
