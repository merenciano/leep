#include "render/commands/create-buffer.h"
#include "render/renderer.h"
#include "glad/glad.h"

#include "core/manager.h"

namespace leep 
{
    void CreateBuffer::executeCommand() const
    {
        Renderer &r = Manager::instance().renderer();

        LEEP_CORE_ASSERT(r.buffers_[buffer_.handle()].cpu_version_ > 0, "This buffer hasn't got any data yet");

        glGenBuffers(1, &(r.buffers_[buffer_.handle()].internal_id_));

        if (buffer_.type() == BufferType::INDEX_BUFFER)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r.buffers_[buffer_.handle()].internal_id_);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                        r.buffers_[buffer_.handle()].count_ * sizeof(uint32_t),
                        (const void*)r.buffers_[buffer_.handle()].data_.indices_,
                        GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        else if (buffer_.type() != BufferType::NONE)
        {
            glBindBuffer(GL_ARRAY_BUFFER, r.buffers_[buffer_.handle()].internal_id_);
            glBufferData(GL_ARRAY_BUFFER,
                        r.buffers_[buffer_.handle()].count_ * sizeof(float),
                        (const void*)r.buffers_[buffer_.handle()].data_.vertices_,
                        GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        
        r.buffers_[buffer_.handle()].gpu_version_ = r.buffers_[buffer_.handle()].cpu_version_; 
    }
}
