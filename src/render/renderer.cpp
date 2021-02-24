#include "renderer.h"
#include "render/materials/pbr.h"
#include "render/materials/full-screen-image.h"
#include "render/materials/skybox.h"
#include "render/materials/equirec-to-cube.h"
#include "render/materials/prefilter-env.h"
#include "render/materials/lut-gen.h"

namespace leep
{
    Renderer::Renderer()
    {
        tex_to_del_.store(-1);
        buf_to_del_.store(-1);
    }
    
    Renderer::~Renderer()
    {

    }

    void Renderer::init()
    {
        // I dont mind them not being together in memory
        // since the correct usage of this engine will be
        // using the diferent materials in order so only one change per frame
        materials_[MaterialType::MT_PBR] = std::make_unique<Pbr>();
        materials_[MaterialType::MT_FULL_SCREEN_IMAGE] = std::make_unique<FullScreenImage>();
        materials_[MaterialType::MT_SKYBOX] = std::make_unique<Skybox>();
        materials_[MaterialType::MT_EQUIREC_TO_CUBE] = std::make_unique<EquirecToCube>();
        materials_[MaterialType::MT_PREFILTER_ENV] = std::make_unique<PrefilterEnv>();
        materials_[MaterialType::MT_LUT_GEN] = std::make_unique<LutGen>();
        
        for (int32_t i = 0; i < MaterialType::MT_MAX; ++i)
        {
            materials_[i]->init();
        }
    }

    void Renderer::addDisplayListToQueue(DisplayList &&dl)
    {
        nxt_frame_q_mtx_.lock();
        next_frame_command_queue_.push_back(std::move(dl));
        nxt_frame_q_mtx_.unlock();
    }

    void Renderer::renderFrame()
    {
        while (!current_frame_commands_.empty())
        {
            DisplayList dl(std::move(current_frame_commands_.front()));
            current_frame_commands_.pop_front();

            // I can't use the range-based for loop here because unique_ptr can't be referenced,
            for (auto it = dl.command_list().cbegin(); it != dl.command_list().cend(); ++it)
            {
                it->get()->executeCommand();
            }
        }
        deleteResources();
    }

    void Renderer::submitFrame()
    {
        LEEP_CORE_ASSERT(current_frame_commands_.empty(), "The current frame command list is not empty!");
        next_frame_command_queue_.swap(current_frame_commands_);
    }

    // TODO IMPORTANT: make a command with this...
    // I dont want opengl calls from renderer class (must be backend agnostic)
    #include <glad/glad.h>
    void Renderer::deleteResources()
    {
        int32_t r = tex_to_del_.load();
        if (r != -1)
        {
            glDeleteTextures(1, (GLuint*)&r);
            tex_to_del_.store(-1);
        }

        r = buf_to_del_.load();
        if (r != -1)
        {
            glDeleteBuffers(1, (GLuint*)&r);
            buf_to_del_.store(-1);
        }
    }
}
