#include "renderer.h"

#include "core/memory/memory.h"
#include "render/display-list.h"
#include "render/display-list-command.h"
#include "render/materials/pbr.h"
#include "render/materials/full-screen-image.h"
#include "render/materials/skybox.h"
#include "render/materials/equirec-to-cube.h"
#include "render/materials/prefilter-env.h"
#include "render/materials/lut-gen.h"

namespace leep {

Renderer::Renderer()
{
    tex_to_del_.store(-1);
    buf_to_del_.store(-1);
}

Renderer::~Renderer()
{
    LEEP_CORE_ERROR("Render destructor called");
}

void Renderer::init()
{
    rq_.init(&(GM.memory()));
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

void Renderer::renderFrame()
{
    for (int32_t i = 0; i < rq_.curr_count_; ++i)
    {
        rq_.curr_queue_[i]->executeCommand();
    }
    deleteResources();
}

void Renderer::submitFrame()
{
    rq_.swapQueues();
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

RenderQueues::RenderQueues()
{
    curr_pool_ = nullptr;
    next_pool_ = nullptr;
    next_offset_ = nullptr;
    curr_queue_ = nullptr;
    next_queue_ = nullptr;
    curr_count_ = 0;
    next_count_ = 0;
}

RenderQueues::~RenderQueues()
{
    LEEP_CORE_ERROR("RenderQueues destructor called");
}

void RenderQueues::init(Memory *m)
{
    LEEP_CORE_ASSERT(curr_pool_ == nullptr,
        "Already initialized.");
    curr_pool_ = (int8_t*)m->alloc(kRenderPoolSize);
    next_pool_ = (int8_t*)m->alloc(kRenderPoolSize);
    next_offset_ = next_pool_;

    curr_queue_ = (DLComm**)m->alloc(kRenderQueueMaxCount * sizeof(int*));
    next_queue_ = (DLComm**)m->alloc(kRenderQueueMaxCount * sizeof(int*));
    curr_count_ = 0;
    next_count_ = 0;
}

void RenderQueues::addDL(DisplayList *dl)
{
    LEEP_CORE_ASSERT(next_count_ + dl->commandListCount() < 
        kRenderQueueMaxCount, "Max queue count reached");
    for (int32_t i = 0; i < dl->commandListCount(); ++i)
    {
        next_queue_[next_count_++] = dl->command_list()[i];
    }
}

} // namespace leep
