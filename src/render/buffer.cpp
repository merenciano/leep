#include "buffer.h"

#include "core/manager.h"
#include "core/common-defs.h"
#include "render/internal-buffer.h"

namespace motoret
{
    Buffer::Buffer()
    {
        handler_ = ConstantValues::UNINITIALIZED_HANDLER;
        type_ = BufferType::NONE;
    }

    Buffer::Buffer(const Buffer &other)
    {
        handler_ = other.handler_;
        type_ = other.type_;
    }

    Buffer& Buffer::operator=(const Buffer &other)
    {
        handler_ = other.handler_;
        type_ = other.type_;
        return *this;
    }

    void Buffer::create(BufferType type)
    {
        MOTORET_ASSERT(handler_ == ConstantValues::UNINITIALIZED_HANDLER, "This handler has been created before");

        type_ = type;
        if (!Manager::instance().renderer().aviable_buffer_vector_positions_.empty())
        {
            // Getting the first element and removing it from the list
            handler_ = Manager::instance().renderer().aviable_buffer_vector_positions_.front();
            Manager::instance().renderer().aviable_buffer_vector_positions_.pop_front();
        }
        else
        {
            InternalBuffer temp;
            Manager::instance().renderer().buffers_.push_back(std::move(temp));
            handler_ = (int32_t)Manager::instance().renderer().buffers_.size() - 1;
        }
    }

    void Buffer::set_data(std::vector<Vertex> &data)
    {
        Manager::instance().renderer().buffers_[handler_].vertices_data_.swap(data);
        Manager::instance().renderer().buffers_[handler_].version_++;
        data.clear();
    }

    void Buffer::set_data(std::vector<uint32_t> &data)
    {
        Manager::instance().renderer().buffers_[handler_].indices_data_.swap(data);
        Manager::instance().renderer().buffers_[handler_].version_++;
        data.clear();
    }

    BufferType Buffer::type() const
    {
        return type_;
    }

    int32_t Buffer::handler() const
    {
        return handler_;
    }

    void Buffer::release()
    {
        handler_ = ConstantValues::DELETED_HANDLER;
        type_ = BufferType::NONE;
    }
}

