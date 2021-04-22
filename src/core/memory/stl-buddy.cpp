#if 0
#include "buddy-alloc-stl.h"
#include "core/memory/memory.h"
#include "core/manager.h"

namespace leep {
    template<typename T>
    T* stl_buddy<T>::allocate(size_t n)
    {
        return static_cast<T*>(GM.memory().generalAlloc(n*sizeof(T)));
    }

    template<typename T>
    void stl_buddy<T>::deallocate(T *p, size_t n)
    {
        GM.memory().generalFree(p);
    }
}
#endif