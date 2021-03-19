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
        }
        else if (buffer_.type() != BufferType::NONE)
        {
            glBindBuffer(GL_ARRAY_BUFFER, r.buffers_[buffer_.handle()].internal_id_);
            glBufferData(GL_ARRAY_BUFFER,
                        r.buffers_[buffer_.handle()].count_ * sizeof(float),
                        (const void*)r.buffers_[buffer_.handle()].data_.vertices_,
                        GL_STATIC_DRAW);
        }
        
        r.buffers_[buffer_.handle()].gpu_version_ = r.buffers_[buffer_.handle()].cpu_version_;

        // Delete ram data now that has been copied into the vram
        buffer_.freeSystemRamData();
    }
}
