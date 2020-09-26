#include "render/renderer.h"
#include "render/materials/pbr.h"
#include "render/materials/plain-color.h"

namespace motoret
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
        // using the diferent materials in order, so there 
        // should be hardly any cache misses
        // (only one each time we change the material)
        materials_[MaterialType::PBR] = std::make_unique<Pbr>();
        materials_[MaterialType::PLAIN_COLOR] = std::make_unique<PlainColor>();
    }

    void Renderer::addDisplayListToQueue(DisplayList &&dl)
    {
        next_frame_command_queue_mtx_.lock();
        next_frame_command_queue_.push_back(std::move(dl));
        next_frame_command_queue_mtx_.unlock();
    }
}