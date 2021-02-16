// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_BUFFER_H__
#define __LEEP_RENDER_BUFFER_H__ 1

#include <stdint.h>
#include <vector>

namespace leep
{
    enum class BufferType
    {
        NONE = 0,
        VERTEX_BUFFER_3P_2UV,
        VERTEX_BUFFER_3P_3N_2UV,
        VERTEX_BUFFER_3P_3N_3T_3B_2UV,
        INDEX_BUFFER,
    };

    class Buffer
    {
    public:
        Buffer();
        Buffer(const Buffer &other);
        Buffer(Buffer &&other) = delete;
        ~Buffer() {}

        Buffer& operator=(const Buffer &other);

        void create();
        void set_data(std::vector<float> &data, BufferType type);
        void set_data(std::vector<uint32_t> &data);
        BufferType type() const;
        int32_t handle() const;
        void release();

    private:
        BufferType type_;
        int32_t handle_; 
    };
}

#endif // __LEEP_RENDER_BUFFER_H__