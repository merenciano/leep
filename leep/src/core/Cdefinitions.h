#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

extern "C" {
#include "mathc.h"
}

#define THE_ASSERT(X) assert(X)
#define THE_LOG_ERROR(FMT, ...) printf("ERROR @ %s(%u): \n\t" FMT "\n", __FILE__, __LINE__, __VA_ARGS__)
#define THE_SLOG_ERROR(FMT) printf("ERROR @ %s(%u): \n\t" FMT "\n", __FILE__, __LINE__)
#define THE_LOG_WARNING(FMT, ...) printf("WARNING @ %s(%u): \n\t" FMT "\n", __FILE__, __LINE__, __VA_ARGS__)
#define THE_SLOG_WARNING(FMT) printf("WARNING @ %s(%u): \n\t" FMT "\n", __FILE__, __LINE__)
#define THE_LOG(FMT, ...) printf(FMT "\n", __VA_ARGS__)
#define THE_SLOG(FMT) printf(FMT "\n")

#define THE_UNINIT -5000
#define THE_DELETED -5001
#define THE_MARKED_FOR_DELETE -5002
#define THE_INVALID -5003

#define THE_BYTE_TO_KB(X) (X / 1024.0f)
#define THE_BYTE_TO_MB(X) (X / 1048576.0f)

typedef int64_t s64;
typedef uint64_t u64;
typedef int32_t s32;
typedef uint32_t u32;
typedef int16_t s16;
typedef uint16_t u16;
typedef int8_t s8;
typedef uint8_t u8;