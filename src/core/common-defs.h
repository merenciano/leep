#ifndef __LEEP_CORE_COMMON_DEFS_H__
#define __LEEP_CORE_COMMON_DEFS_H__ 1

#include "core/logger.h"

#ifdef LEEP_DEBUG 
    #define LEEP_ASSERT(x, ...)      { if(!(x)) { LEEP_ERROR("Assertion failed: {0}",__VA_ARGS__); abort(); }}
    #define LEEP_CORE_ASSERT(x, ...) { if(!(x)) { LEEP_CORE_ERROR("Assertion failed: {0}",__VA_ARGS__); abort(); }}
#else
    #define LEEP_ASSERT(x, ...) 
    #define LEEP_CORE_ASSERT(x, ...) 
#endif 

#define MAX_INT32_VALUE = 2147483647;

#define GM Manager::instance()

namespace leep 
{
    enum ConstantValues     // Not an enum class because it is used to assign int32_t handlers
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

    const uint32_t kEntitiesPerChunk = 128; 

    constexpr bool IsPow2(uint32_t n) 
    { 
        uint8_t count = 0; 
        while (n) { 
            count += n & 1; 
            n >>= 1; 
        } 
        return count == 1; 
    } 

    constexpr int32_t EntityI(int32_t entity_id)
    {
        // %kEntitiesPerChunk
        return entity_id&(kEntitiesPerChunk - 1);
    }

    constexpr int32_t ChunkI(int32_t entity_id)
    {
        return entity_id/kEntitiesPerChunk;
    }

}


#endif // __LEEP_CORE_COMMON_DEFS_H__
