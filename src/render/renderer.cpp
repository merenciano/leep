#include "renderer.h"
#include "render/materials/pbr.h"
#include "render/materials/plain-color.h"

namespace leep
{
    Renderer::Renderer()
    {

    }
    
    Renderer::~Renderer()
    {

    }

    void Renderer::init()
    {
        // I dont mind them not being together in memory
        // since the correct usage of this engine will be
        // using the diferent materials in order
        materials_[MaterialType::MT_PBR] = std::make_unique<Pbr>();
        materials_[MaterialType::MT_PLAIN_COLOR] = std::make_unique<PlainColor>();
        
        for (int32_t i = 0; i < MaterialType::MT_MAX; ++i)
        {
            materials_[i]->init();
        }
    }

    void Renderer::addDisplayListToQueue(DisplayList &&dl)
    {
        next_frame_command_queue_mtx_.lock();
        next_frame_command_queue_.push_back(std::move(dl));
        next_frame_command_queue_mtx_.unlock();
    }

    void Renderer::renderFrame()
    {
        while (!current_frame_commands_.empty())
        {
            DisplayList dl(std::move(current_frame_commands_.front()));
            current_frame_commands_.pop_front();

            // Here I can't use the range-based for loop because unique_ptr can't be referenced,
            // so I have to use this ugly loop instead
            for (auto it = dl.command_list().cbegin(); it != dl.command_list().cend(); ++it)
            {
                it->get()->executeCommand();
            }
        }
    }

    void Renderer::submitFrame()
    {
        LEEP_CORE_ASSERT(current_frame_commands_.empty(), "The current frame command list is not empty!");
        next_frame_command_queue_.swap(current_frame_commands_);
    }
}