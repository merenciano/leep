#include "render/commands/draw.h"

#include "core/common-defs.h"
#include "core/manager.h"

#include "glad/glad.h"

namespace motoret
{
    void Draw::executeCommand() const
    {
        Renderer &r = Manager::instance().renderer();
        int32_t vertex_handler = geometry_.vertex_buffer().handler();
        int32_t index_handler = geometry_.index_buffer().handler();

        MOTORET_CORE_ASSERT(vertex_handler == ConstantValues::UNINITIALIZED_HANDLER,
            "You are trying to draw with an uninitialized vertex buffer");

        MOTORET_CORE_ASSERT(index_handler == ConstantValues::UNINITIALIZED_HANDLER,
            "You are trying to draw with an uninitialized index buffer");

        MOTORET_CORE_ASSERT(r->buffers_[vertex_handler].version_ > 0,
            "Vertex buffer without data");

        MOTORET_CORE_ASSERT(r->buffers_[index_handler].version_ > 0, 
            "Index buffer without data");

        MOTORET_CORE_ASSERT(material_.type() == MaterialType::NONE,
            "Material type not setted");

        // Create the OpenGL vertex buffer if it has not been created yet
        if (r.buffers_[vertex_handler].gpu_version_ == 0)
        {
            glGenBuffers(1, &(r.buffers_[vertex_handler].internal_id_));
            glBindBuffer(GL_ARRAY_BUFFER, r.buffers_[vertex_handler].internal_id_);
            glBufferData(GL_ARRAY_BUFFER,
                r.buffers_[vertex_handler].data_.vertices.size() * sizeof(Vertex),
                (const void*)r.buffers_[vertex_handler].data_.vertices.data(),
                GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            r.buffers_[vertex_handler].gpu_version_ = r.buffers_[vertex_handler].version_; 
        }

        // Create the OpenGL index buffer if it has not been created yet
        if (r.buffers_[index_handler].gpu_version_ == 0)
        {
            glGenBuffers(1, &(r.buffers_[index_handler].internal_id_));
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r.buffers_[index_handler].internal_id_);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                r.buffers_[index_handler].data_.indices.size() * sizeof(uint32_t),
                (const void*)r.buffers_[index_handler].data_.indices.data(),
                GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            r.buffers_[index_handler].gpu_version_ = r.buffers_[index_handler].version_; 
        }
    }
    // VOY POR AQUI...
}