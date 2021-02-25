#include "buffer.h"

#include "core/manager.h"
#include "core/common-defs.h"
#include "render/renderer.h"
#include "render/internal-resources.h"

namespace leep
{
    Buffer::Buffer()
    {
        handle_ = CommonDefs::UNINIT_HANDLE;
        type_ = BufferType::NONE;
    }

    Buffer::Buffer(const Buffer &other)
    {
        handle_ = other.handle_;
        type_ = other.type_;
    }

    Buffer& Buffer::operator=(const Buffer &other)
    {
        handle_ = other.handle_;
        type_ = other.type_;
        return *this;
    }

    void Buffer::create()
    {
        LEEP_ASSERT(handle_ == CommonDefs::UNINIT_HANDLE, "This handler has been created before");

        if (!Manager::instance().renderer().aviable_buffer_pos_.empty())
        {
            // Getting the first element and removing it from the list
            handle_ = Manager::instance().renderer().aviable_buffer_pos_.front();
            Manager::instance().renderer().aviable_buffer_pos_.pop_front();
        }
        else
        {
            Manager::instance().renderer().buffers_.emplace_back();
            handle_ = (int32_t)Manager::instance().renderer().buffers_.size() - 1;
        }
    }

    void Buffer::set_data(std::vector<float> &data, BufferType type)
    {
        type_ = type;
        Manager::instance().renderer().buffers_[handle_].vertices_data_.swap(data);
        Manager::instance().renderer().buffers_[handle_].cpu_version_++;
        data.clear();
    }

    void Buffer::set_data(std::vector<uint32_t> &data)
    {
        type_ = BufferType::INDEX_BUFFER;
        Manager::instance().renderer().buffers_[handle_].indices_data_.swap(data);
        Manager::instance().renderer().buffers_[handle_].cpu_version_++;
        data.clear();
    }

    BufferType Buffer::type() const
    {
        return type_;
    }

    int32_t Buffer::handle() const
    {
        return handle_;
    }

    void Buffer::release()
    {
        if (handle_ >= 0)
        {
            GM.renderer().buffers_[handle_].cpu_version_ = CommonDefs::DELETED_GPU_RESOURCE;
            GM.renderer().buffers_[handle_].gpu_version_ = CommonDefs::DELETED_GPU_RESOURCE;

            handle_ = CommonDefs::DELETED_HANDLE;
        }
    }
}

