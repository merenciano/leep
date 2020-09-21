#ifndef __MOTORET_CORE_COMMON_DEFS_H__
#define __MOTORET_CORE_COMMON_DEFS_H__ 1

#include "core/logger.h"

#ifdef MOTORET_DEBUG 
    #define MOTORET_ASSERT(x, ...)      { if(!(x)) { MOTORET_ERROR("Assertion failed: {0}",__VA_ARGS__); abort(); }}
    #define MOTORET_CORE_ASSERT(x, ...) { if(!(x)) { MOTORET_CORE_ERROR("Assertion failed: {0}",__VA_ARGS__); abort(); }}
#else
    #define MOTORET_ASSERT(x, ...) 
    #define MOTORET_CORE_ASSERT(x, ...) 
#endif 

#define MAX_INT32_VALUE = 2147483647;

namespace motoret
{
    enum ConstantValues
    {
        UNINITIALIZED_HANDLER     = -5000,
        DELETED_HANDLER           = -5001,

        INVALID_VERSION           = -6000,
        INVALID_GPU_VERSION       = -6001,

        UNINITIALIZED_INTERNAL_ID = 900000000,
        DELETED_INTERNAL_ID       = 900000001,
    };

    struct Vertex
    {
        float px;
        float py;
        float pz;
        float nx;
        float ny;
        float nz;
        float tx;
        float ty;
    };
}


#endif // __MOTORET_CORE_COMMON_DEFS_H__
