#ifndef __MOTORET_RENDER_INTERNAL_BUFFER_H__
#define __MOTORET_RENDER_INTERNAL_BUFFER_H__ 1

#include "core/common-defs.h"
#include "core/logger.h"
#include <stdint.h>

namespace motoret
{
    struct InternalBuffer
    {
        InternalBuffer()
        {
            internal_id_ = ConstantValues::UNINITIALIZED_INTERNAL_ID;
            version_ = 0;
            gpu_version_ = 0;
        }

        InternalBuffer(const InternalBuffer& other) = delete;

        InternalBuffer(InternalBuffer&& other)
        {
            internal_id_ = other.internal_id_;
            other.internal_id_ = ConstantValues::UNINITIALIZED_INTERNAL_ID;
            version_ = other.version_;
            other.version_ = 0;
            gpu_version_ = other.gpu_version_;
            other.gpu_version_ = 0;
            std::swap(data_.vertices, other.data_.vertices);
        }

        ~InternalBuffer()
        {
            MOTORET_CORE_INFO("InternalBuffer data freed");
            data_.vertices.clear();
        }

        void markAsDeleted()
        {
            version_ = ConstantValues::INVALID_VERSION;
            gpu_version_ = ConstantValues::INVALID_GPU_VERSION;
        }

        uint32_t internal_id_;
        int32_t version_;
        int32_t gpu_version_;
        union Data
        {
            Data() {}
            ~Data() {}
            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;
        } data_;
    };
}

#endif // __MOTORET_RENDER_INTERNAL_BUFFER_H__