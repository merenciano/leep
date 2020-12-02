#include "render/commands/draw.h"

#include "core/common-defs.h"
#include "core/manager.h"

#include "glad/glad.h"

namespace leep
{
    void Draw::executeCommand() const
    {
        Renderer &r = Manager::instance().renderer();
        int32_t vertex_handler = geometry_.vertex_buffer().handler();
        int32_t index_handler = geometry_.index_buffer().handler();

        LEEP_CORE_ASSERT(vertex_handler != ConstantValues::UNINITIALIZED_HANDLER,
            "You are trying to draw with an uninitialized vertex buffer");

        LEEP_CORE_ASSERT(index_handler != ConstantValues::UNINITIALIZED_HANDLER,
            "You are trying to draw with an uninitialized index buffer");

        LEEP_CORE_ASSERT(r.buffers_[vertex_handler].version_ > 0,
            "Vertex buffer without data");

        LEEP_CORE_ASSERT(r.buffers_[index_handler].version_ > 0, 
            "Index buffer without data");

        LEEP_CORE_ASSERT(material_.type() != MaterialType::MT_NONE,
            "Material type not setted");

        // Set the uniforms
        r.materials_[material_.type()]->useMaterialData(material_);

        // Create the OpenGL vertex buffer if it has not been created yet
        if (r.buffers_[vertex_handler].gpu_version_ == 0)
        {
            glGenBuffers(1, &(r.buffers_[vertex_handler].internal_id_));
            glBindBuffer(GL_ARRAY_BUFFER, r.buffers_[vertex_handler].internal_id_);
            glBufferData(GL_ARRAY_BUFFER,
                r.buffers_[vertex_handler].vertices_data_.size() * sizeof(Vertex),
                (const void*)r.buffers_[vertex_handler].vertices_data_.data(),
                GL_STATIC_DRAW);
            r.buffers_[vertex_handler].gpu_version_ = r.buffers_[vertex_handler].version_; 
        }
        else
        {
            glBindBuffer(GL_ARRAY_BUFFER,
                r.buffers_[geometry_.vertex_buffer().handler()].internal_id_);
        }

        // Create the OpenGL index buffer if it has not been created yet
        if (r.buffers_[index_handler].gpu_version_ == 0)
        {
            glGenBuffers(1, &(r.buffers_[index_handler].internal_id_));
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r.buffers_[index_handler].internal_id_);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                r.buffers_[index_handler].indices_data_.size() * sizeof(uint32_t),
                (const void*)r.buffers_[index_handler].indices_data_.data(),
                GL_STATIC_DRAW);
            r.buffers_[index_handler].gpu_version_ = r.buffers_[index_handler].version_; 
        }
        else
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                r.buffers_[geometry_.index_buffer().handler()].internal_id_);
        }

        // Remove from here
        glEnable(GL_DEPTH_TEST);

        // In order to keep things simple, at this moment the engine
        // only supports 3P, 3N, 2UV vertices
        GLint attrib_pos = glGetAttribLocation(
            r.materials_[material_.type()]->internal_id(), "a_position");
        glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
            8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(attrib_pos);

        attrib_pos = glGetAttribLocation(
            r.materials_[material_.type()]->internal_id(), "a_normal"); 
        glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
            8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(attrib_pos);

        attrib_pos = glGetAttribLocation(
            r.materials_[material_.type()]->internal_id(), "a_uv");
        glVertexAttribPointer(attrib_pos, 2, GL_FLOAT, GL_FALSE,
            8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(attrib_pos);

        uint32_t index_count = (uint32_t)r.buffers_[geometry_.index_buffer().handler()].indices_data_.size();
        glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}