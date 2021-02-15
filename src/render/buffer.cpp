#include "buffer.h"

#include "core/manager.h"
#include "core/common-defs.h"
#include "render/renderer.h"
#include "render/internal-resources.h"

namespace leep
{
    Buffer::Buffer()
    {
        handle_ = ConstantValues::UNINITIALIZED_HANDLER;
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

    void Buffer::create(BufferType type)
    {
        LEEP_ASSERT(handle_ == ConstantValues::UNINITIALIZED_HANDLER, "This handler has been created before");

        type_ = type;
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

    void Buffer::set_data(std::vector<Vertex> &data)
    {
        Manager::instance().renderer().buffers_[handle_].vertices_data_.swap(data);
        Manager::instance().renderer().buffers_[handle_].version_++;
        data.clear();
    }

    void Buffer::set_data(std::vector<uint32_t> &data)
    {
        Manager::instance().renderer().buffers_[handle_].indices_data_.swap(data);
        Manager::instance().renderer().buffers_[handle_].version_++;
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
        handle_ = ConstantValues::DELETED_HANDLER;
        type_ = BufferType::NONE;
    }
}

