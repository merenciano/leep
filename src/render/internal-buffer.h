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

        InternalBuffer(InternalBuffer&& other) noexcept
        {
            internal_id_ = other.internal_id_;
            other.internal_id_ = ConstantValues::UNINITIALIZED_INTERNAL_ID;
            version_ = other.version_;
            other.version_ = 0;
            gpu_version_ = other.gpu_version_;
            other.gpu_version_ = 0;
            vertices_data_.swap(other.vertices_data_);
            indices_data_.swap(other.indices_data_);
        }

        ~InternalBuffer()
        {
            MOTORET_CORE_INFO("InternalBuffer data freed");
        }

        void markAsDeleted()
        {
            version_ = ConstantValues::INVALID_VERSION;
            gpu_version_ = ConstantValues::INVALID_GPU_VERSION;
        }

        uint32_t internal_id_;
        int32_t version_;
        int32_t gpu_version_;

        //  TODO: I don't like having one unused vector in every buffer
        //  but I didn't be able to make it work with unions because
        //  of the exceptions it threw, I whink multiple std::vectors
        //  inside an union does not work well, but I need some research on this
        std::vector<Vertex> vertices_data_;
        std::vector<uint32_t> indices_data_;
    };
}

#endif // __MOTORET_RENDER_INTERNAL_BUFFER_H__