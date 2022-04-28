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
	    handle_ = THE_UNINIT;
        //handle_ = CommonDefs::UNINIT_HANDLE;
        //type_ = BufferType::NONE;
    }

    Buffer::Buffer(const Buffer &other)
    {
        handle_ = other.handle_;
        //type_ = other.type_;
    }

    Buffer& Buffer::operator=(const Buffer &other)
    {
        handle_ = other.handle_;
        //type_ = other.type_;
        return *this;
    }

    void Buffer::create()
    {
	    handle_ = THE_CreateBuffer();
	    /*
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
        }*/
    }

    THE_BufferType ToCType(BufferType t)
    {
	    switch (t)
	    {
	    case BufferType::INDEX_BUFFER: return THE_BUFFER_INDEX;
	    case BufferType::NONE: return THE_BUFFER_NONE;
	    case BufferType::VERTEX_BUFFER_3P: return THE_BUFFER_VERTEX_3P;
	    case BufferType::VERTEX_BUFFER_3P_2UV: return THE_BUFFER_VERTEX_3P_2UV;
	    case BufferType::VERTEX_BUFFER_3P_3N: return THE_BUFFER_VERTEX_3P_3N;
	    case BufferType::VERTEX_BUFFER_3P_3N_2UV: return THE_BUFFER_VERTEX_3P_3N_2UV;
	    case BufferType::VERTEX_BUFFER_3P_3N_3T_3B_2UV: return THE_BUFFER_VERTEX_3P_3N_3T_3B_2UV;
	    }
    }

    BufferType ToJEJEType(THE_BufferType t)
    {
	    switch (t)
	    {
	    case THE_BUFFER_INDEX: return BufferType::INDEX_BUFFER;
	    case THE_BUFFER_NONE: return BufferType::NONE;
	    case THE_BUFFER_VERTEX_3P: return BufferType::VERTEX_BUFFER_3P;
	    case THE_BUFFER_VERTEX_3P_2UV: return BufferType::VERTEX_BUFFER_3P_2UV;
	    case THE_BUFFER_VERTEX_3P_3N: return BufferType::VERTEX_BUFFER_3P_3N;
	    case THE_BUFFER_VERTEX_3P_3N_2UV: return BufferType::VERTEX_BUFFER_3P_3N_2UV;
	    case THE_BUFFER_VERTEX_3P_3N_3T_3B_2UV: return BufferType::VERTEX_BUFFER_3P_3N_3T_3B_2UV;
	    }
    }

    void Buffer::create(float *d, uint32_t c, BufferType t)
    {
	    handle_ = THE_CreateBuffer();
	    THE_SetBufferData(handle_, d, c, ToCType(t));
	    /*LEEP_ASSERT(handle_ == CommonDefs::UNINIT_HANDLE, "This handler has been created before");

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

        set_data(d, c, t);*/
      
    }

    void Buffer::create(uint32_t* d, uint32_t c)
    {
	    handle_ = THE_CreateBuffer();
	    THE_SetBufferData(handle_, d, c, THE_BUFFER_INDEX);
        /*LEEP_ASSERT(handle_ == CommonDefs::UNINIT_HANDLE, "This handler has been created before");

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

        set_data(d, c);*/
    }

    void Buffer::set_data(float *d, uint32_t c, BufferType t)
    {
	    THE_SetBufferData(handle_, d, c, ToCType(t));
	    /* LEEP_CORE_ASSERT(GM.renderer().buffers_[handle_].data_.vertices_
             == nullptr, "There is data to be freed before setting new one");
        type_ = t;
        GM.renderer().buffers_[handle_].count_ = c;
        GM.renderer().buffers_[handle_].data_.vertices_ = d;
        GM.renderer().buffers_[handle_].cpu_version_++;*/
       
    }

    void Buffer::set_data(uint32_t *d, uint32_t c)
    {
	    THE_SetBufferData(handle_, d, c, THE_BUFFER_INDEX);
        /*LEEP_CORE_ASSERT(GM.renderer().buffers_[handle_].data_.indices_
             == 0, "There is data to be freed before setting new one");
        type_ = BufferType::INDEX_BUFFER;
        GM.renderer().buffers_[handle_].count_ = c;
        GM.renderer().buffers_[handle_].data_.indices_ = d;
        GM.renderer().buffers_[handle_].cpu_version_++;*/
    }

    BufferType Buffer::type() const
    {

        return ToJEJEType(THE_GetBufferType(handle_));
    }

    int32_t Buffer::handle() const
    {
        return handle_;
    }

    void Buffer::release()
    {
	    THE_ReleaseBuffer(handle_);
       /*if (handle_ >= 0)
        {
            GM.renderer().buffers_[handle_].cpu_version_ = CommonDefs::DELETED_GPU_RESOURCE;
            GM.renderer().buffers_[handle_].gpu_version_ = CommonDefs::DELETED_GPU_RESOURCE;
            GM.memory().generalFree(GM.renderer().buffers_[handle_].data_.vertices_);
            GM.renderer().buffers_[handle_].data_.vertices_ = nullptr;

            handle_ = CommonDefs::DELETED_HANDLE;
        }*/ 
    }

    void Buffer::freeSystemRamData() const
    {
	    THE_FreeBufferData(handle_);
	    /*if (GM.renderer().buffers_[handle_].data_.vertices_)
        {
            GM.memory().generalFree(GM.renderer().buffers_[handle_].data_.vertices_);
        }*/
        
    }
}
