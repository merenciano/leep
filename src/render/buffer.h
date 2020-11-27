// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_BUFFER_H__
#define __LEEP_RENDER_BUFFER_H__ 1

#include "core/common-defs.h"
#include <stdint.h>
#include <vector>

namespace leep
{
    enum class BufferType
    {
        NONE = 0,
        VERTEX_BUFFER,
        INDEX_BUFFER,
    };

    class Buffer
    {
    public:
        Buffer();
        Buffer(BufferType t) : type_(t) { handler_ = ConstantValues::UNINITIALIZED_HANDLER; }
        Buffer(const Buffer &other);
        Buffer(Buffer &&other) = delete;
        ~Buffer() {}

        Buffer& operator=(const Buffer &other);

        void create(BufferType type);
        void set_data(std::vector<Vertex> &data);
        void set_data(std::vector<float> &data);
        void set_data(std::vector<uint32_t> &data);
        BufferType type() const;
        int32_t handler() const;
        void release();

    private:
        BufferType type_;
        int32_t handler_; 
    };
}

#endif // __LEEP_RENDER_BUFFER_H__