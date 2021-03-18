#include "buffer.h"

#include "core/manager.h"
#include "core/common-defs.h"
#include "core/memory/memory.h"
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
            handle_ = GM.renderer().addBuf();
        }
    }

    void Buffer::create(float *d, uint32_t c, BufferType t)
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
            handle_ = GM.renderer().addBuf();
        }

        set_data(d, c, t);
    }

    void Buffer::create(uint32_t* d, uint32_t c)
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
            handle_ = GM.renderer().addBuf();
        }

        set_data(d, c);
    }

    void Buffer::set_data(float *d, uint32_t c, BufferType t)
    {
        LEEP_CORE_ASSERT(GM.renderer().buffers_[handle_].data_.vertices_
             == nullptr, "There is data to be freed before setting new one");
        type_ = t;
        GM.renderer().buffers_[handle_].count_ = c;
        GM.renderer().buffers_[handle_].data_.vertices_ = d;
        GM.renderer().buffers_[handle_].cpu_version_++;
    }

    void Buffer::set_data(uint32_t *d, uint32_t c)
    {
        LEEP_CORE_ASSERT(GM.renderer().buffers_[handle_].data_.indices_
             == nullptr, "There is data to be freed before setting new one");
        type_ = BufferType::INDEX_BUFFER;
        GM.renderer().buffers_[handle_].count_ = c;
        GM.renderer().buffers_[handle_].data_.indices_ = d;
        GM.renderer().buffers_[handle_].cpu_version_++;
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
            GM.memory().generalFree(GM.renderer().buffers_[handle_].data_.vertices_);
            GM.renderer().buffers_[handle_].data_.vertices_ = nullptr;

            handle_ = CommonDefs::DELETED_HANDLE;
        }
    }

    void Buffer::freeSystemRamData() const
    {
        if (GM.renderer().buffers_[handle_].data_.vertices_)
        {
            GM.memory().generalFree(GM.renderer().buffers_[handle_].data_.vertices_);
        }
    }
}
