/**
 * @file buffer.h
 * @author Lucas Merenciano Martinez (lucasmermar@gmail.com)
 * @brief GPU Buffer handle class
 * @version 0.1
 */

#ifndef __LEEP_RENDER_BUFFER_H__
#define __LEEP_RENDER_BUFFER_H__

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
        void create(float* data, uint32_t count, BufferType type);
        void create(uint32_t* data, uint32_t count);
        void set_data(float *data, uint32_t count, BufferType type);
        void set_data(uint32_t *data, uint32_t count);
        BufferType type() const;
        int32_t handle() const;
        void freeSystemRamData() const;
        void release();

    private:
        BufferType type_;
        int32_t handle_; 
    };
}

#endif // __LEEP_RENDER_BUFFER_H__
