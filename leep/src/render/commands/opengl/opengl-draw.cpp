#include "render/commands/draw.h"

#include "core/common-defs.h"
#include "core/manager.h"
#include "render/renderer.h"
#include "render/commands/create-buffer.h"

#include <glad/glad.h>

namespace leep
{
    void Draw::executeCommand() const
    {
        Renderer& r = GM.renderer();
        int32_t vertex_handle = geometry_.vertex_buffer().handle();
        int32_t index_handle = geometry_.index_buffer().handle();

        LEEP_CORE_ASSERT(vertex_handle !=CommonDefs::UNINIT_HANDLE,
            "You are trying to draw with an uninitialized vertex buffer");

        LEEP_CORE_ASSERT(index_handle != CommonDefs::UNINIT_HANDLE,
            "You are trying to draw with an uninitialized index buffer");

        LEEP_CORE_ASSERT(r.buffers_[vertex_handle].cpu_version_ > 0,
            "Vertex buffer without data");

        LEEP_CORE_ASSERT(r.buffers_[index_handle].cpu_version_ > 0,
            "Index buffer without data");

        LEEP_CORE_ASSERT(material_.type() != MaterialType::MT_NONE,
            "Material type not setted");

        // Set the uniforms
        r.gpu_materials_[material_.type()].useMaterialData(material_);

        // Create the OpenGL vertex buffer if it has not been created yet
        if (r.buffers_[vertex_handle].gpu_version_ == 0)
        {
            CreateBuffer().set_buffer(geometry_.vertex_buffer()).executeCommand();
        }
        else
        {
            glBindBuffer(GL_ARRAY_BUFFER,
                r.buffers_[geometry_.vertex_buffer().handle()].internal_id_);
        }

        // Create the OpenGL index buffer if it has not been created yet
        if (r.buffers_[index_handle].gpu_version_ == 0)
        {
            CreateBuffer().set_buffer(geometry_.index_buffer()).executeCommand();
        }
        else
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                r.buffers_[geometry_.index_buffer().handle()].internal_id_);
        }

        switch(geometry_.vertex_buffer().type())
        {
            case BufferType::VERTEX_BUFFER_3P_3N:
            {
                // POSITION
                GLint attrib_pos = glGetAttribLocation(
                        r.gpu_materials_[material_.type()].internal_id(), "a_position");
                glEnableVertexAttribArray(attrib_pos);
                glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
                    6 * sizeof(float), (void*)0);
                glVertexAttribDivisor(attrib_pos, 0);

                // NORMAL
                attrib_pos = glGetAttribLocation(
                        r.gpu_materials_[material_.type()].internal_id(), "a_normal");
                glEnableVertexAttribArray(attrib_pos);
                glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
                    6 * sizeof(float), (void*)(3 * sizeof(float)));
                glVertexAttribDivisor(attrib_pos, 0);
                break;
            }

            case BufferType::VERTEX_BUFFER_3P_2UV:
            {
                // POSITION
                GLint attrib_pos = glGetAttribLocation(
                        r.gpu_materials_[material_.type()].internal_id(), "a_position");
                glEnableVertexAttribArray(attrib_pos);
                glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
                    5 * sizeof(float), (void*)0);
                glVertexAttribDivisor(attrib_pos, 0);

                // UV
                attrib_pos = glGetAttribLocation(
                        r.gpu_materials_[material_.type()].internal_id(), "a_uv");
                glEnableVertexAttribArray(attrib_pos);
                glVertexAttribPointer(attrib_pos, 2, GL_FLOAT, GL_FALSE,
                    5 * sizeof(float), (void*)(3 * sizeof(float)));
                glVertexAttribDivisor(attrib_pos, 0);
                break;
            }
            case BufferType::VERTEX_BUFFER_3P_3N_2UV:
            {
                // POSITION
                GLint attrib_pos = glGetAttribLocation(
                        r.gpu_materials_[material_.type()].internal_id(), "a_position");
                glEnableVertexAttribArray(attrib_pos);
                glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
                    8 * sizeof(float), (void*)0);
                glVertexAttribDivisor(attrib_pos, 0);

                // NORMAL
                attrib_pos = glGetAttribLocation(
                        r.gpu_materials_[material_.type()].internal_id(), "a_normal");
                glEnableVertexAttribArray(attrib_pos);
                glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
                    8 * sizeof(float), (void*)(3 * sizeof(float)));
                glVertexAttribDivisor(attrib_pos, 0);

                // UV
                attrib_pos = glGetAttribLocation(
                        r.gpu_materials_[material_.type()].internal_id(), "a_uv");
                glEnableVertexAttribArray(attrib_pos);
                glVertexAttribPointer(attrib_pos, 2, GL_FLOAT, GL_FALSE,
                    8 * sizeof(float), (void*)(6 * sizeof(float)));
                glVertexAttribDivisor(attrib_pos, 0);
                break;
            }

            case BufferType::VERTEX_BUFFER_3P_3N_3T_3B_2UV:
            {
                // POSITION
                GLint attrib_pos = glGetAttribLocation(
                        r.gpu_materials_[material_.type()].internal_id(), "a_position");
                glEnableVertexAttribArray(attrib_pos);
                glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
                    14 * sizeof(float), (void*)0);
                glVertexAttribDivisor(attrib_pos, 0);

                // NORMAL
                attrib_pos = glGetAttribLocation(
                        r.gpu_materials_[material_.type()].internal_id(), "a_normal");
                glEnableVertexAttribArray(attrib_pos);
                glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
                    14 * sizeof(float), (void*)(3 * sizeof(float)));
                glVertexAttribDivisor(attrib_pos, 0);

                // TANGENT
                attrib_pos = glGetAttribLocation(
                        r.gpu_materials_[material_.type()].internal_id(), "a_tangent");
                glEnableVertexAttribArray(attrib_pos);
                glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
                    14 * sizeof(float), (void*)(6 * sizeof(float)));
                glVertexAttribDivisor(attrib_pos, 0);
                
                // BITANGENT
                attrib_pos = glGetAttribLocation(
                        r.gpu_materials_[material_.type()].internal_id(), "a_bitangent");
                glEnableVertexAttribArray(attrib_pos);
                glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
                    14 * sizeof(float), (void*)(9 * sizeof(float)));
                glVertexAttribDivisor(attrib_pos, 0);

                // UV
                attrib_pos = glGetAttribLocation(
                        r.gpu_materials_[material_.type()].internal_id(), "a_uv");
                glEnableVertexAttribArray(attrib_pos);
                glVertexAttribPointer(attrib_pos, 2, GL_FLOAT, GL_FALSE,
                    14 * sizeof(float), (void*)(12 * sizeof(float)));
                glVertexAttribDivisor(attrib_pos, 0);
                break;
            }
            default:
                LEEP_CORE_ERROR("Wrong vertex buffer for draw command.");
                break;
        }

        if (inst_count_ > 1)
        {
            LEEP_CORE_ASSERT(inst_attr_.handle() != CommonDefs::UNINIT_HANDLE, "Instance count must be greater than one.");
            LEEP_CORE_ASSERT(inst_attr_.type() == BufferType::VERTEX_BUFFER_3P,
                "The instance attributes buffer has the wrong type.");

            if (r.buffers_[inst_attr_.handle()].gpu_version_ == 0)
            {
                CreateBuffer().set_buffer(inst_attr_).executeCommand();
            }
            else
            {
                glBindBuffer(GL_ARRAY_BUFFER,
                    r.buffers_[inst_attr_.handle()].internal_id_);
            }

            GLint attrib_pos = glGetAttribLocation(
                    r.gpu_materials_[material_.type()].internal_id(), "a_offset");
            glEnableVertexAttribArray(attrib_pos);
            glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
                3 * sizeof(float), (void*)0);
            glVertexAttribDivisor(attrib_pos, 1);
        }

        uint32_t index_count = 
            (uint32_t)r.buffers_[geometry_.index_buffer().handle()].count_;
        glDrawElementsInstanced(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0, inst_count_);
    }
}