#include "render/create-buffer.h"
#include "glad/glad.h"

#include "core/manager.h"

namespace motoret
{
    void CreateBuffer::executeCommand() const
    {
        Renderer &r = Manager::instance().renderer();

        MOTORET_CORE_ASSERT(r.buffers_[buffer_.handler()].version_ > 0, "This buffer hasn't got any data yet");

        glGenBuffers(1, &(r.buffers_[buffer_.handler()].internal_id_));

        if (buffer_.type() == BufferType::VERTEX_BUFFER)
        {
            glBindBuffer(GL_ARRAY_BUFFER, r.buffers_[buffer_.handler()].internal_id_);
            glBufferData(GL_ARRAY_BUFFER,
                        r.buffers_[buffer_.handler()].data_.vertices.size() * sizeof(Vertex),
                        (const void*)r.buffers_[buffer_.handler()].data_.vertices.data(),
                        GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        else if (buffer_.type() == BufferType::INDEX_BUFFER)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r.buffers_[buffer_.handler()].internal_id_);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                        r.buffers_[buffer_.handler()].data_.indices.size() * sizeof(uint32_t),
                        (const void*)r.buffers_[buffer_.handler()].data_.indices.data(),
                        GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        
        r.buffers_[buffer_.handler()].gpu_version_ = r.buffers_[buffer_.handler()].version_; 
    }
}
