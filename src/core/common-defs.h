#ifndef __LEEP_CORE_COMMON_DEFS_H__
#define __LEEP_CORE_COMMON_DEFS_H__ 1

#include "core/logger.h"

#include <glm/glm.hpp>

#ifdef LEEP_DEBUG 
    #define LEEP_ASSERT(x, ...)      { if(!(x)) { LEEP_ERROR("Assertion failed: {0}",__VA_ARGS__); abort(); }}
    #define LEEP_CORE_ASSERT(x, ...) { if(!(x)) { LEEP_CORE_ERROR("Assertion failed: {0}",__VA_ARGS__); abort(); }}
    #define LEEP_CHECK_RESOURCE(r) LEEP_CORE_ASSERT(r.handle() != CommonDefs::UNINIT_HANDLE, "Resource not created")
#else
    #define LEEP_ASSERT(x, ...) 
    #define LEEP_CORE_ASSERT(x, ...) 
    #define LEEP_CHECK_RESOURCE(r)
#endif 

#define MAX_INT32_VALUE = 2147483647

float constexpr ByteToKilo(size_t x) 
{
    return x / 1024.0f;
}

float constexpr ByteToMega(size_t x)
{
    return ByteToKilo(x) / 1024.0f;
}

#define BYTE_TO_MEGA(x) (BYTE_TO_KILO(x) / 1024.0f)

namespace leep 
{
    enum CommonDefs     // Not an enum class because it is used to assign int32_t handlers
    {
        UNINIT_HANDLE           = -5000,
        DELETED_HANDLE          = -5001,

        INVALID_VERSION         = -6000,
        INVALID_GPU_VERSION     = -6001,
        DELETED_GPU_RESOURCE    = -6002,

        MARKED_FOR_DELETE       = -181020,

        UNINIT_INTERNAL_ID      = 900000000,
        DELETED_INTERNAL_ID     = 900000001,
    };

    enum class EntityType
    {
        NONE,
        RENDERABLE,
        FALLING_CUBE,
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
