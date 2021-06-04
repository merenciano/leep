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
    sun_dir_intensity_ = glm::vec4(1.0f, -1.0f, 0.0f, 1.0f);
}

Renderer::~Renderer()
{
    LEEP_CORE_ERROR("Render destructor called");
}

void Renderer::init()
{
    rq_.init(&(GM.memory()));
    textures_ = (InternalTexture*)GM.memory().persistentAlloc(sizeof(InternalTexture)*kMaxTextures);
    buffers_ = (InternalBuffer*)GM.memory().persistentAlloc(sizeof(InternalBuffer)*kMaxBuffers);
    materials_ = (InternalMaterial**)GM.memory().persistentAlloc(MT_MAX * sizeof(int*));
    mat_pool_ = (int8_t*)GM.memory().persistentAlloc(kMatPoolSize);
    mat_offset_ = mat_pool_;
    buf_count_ = 0;
    tex_count_ = 0;


    // I dont mind them not being together in memory
    // since the correct usage of this engine will be
    // using the diferent materials in order so only one change per frame
    materials_[MaterialType::MT_PBR] = matAlloc<Pbr>();
    materials_[MaterialType::MT_FULL_SCREEN_IMAGE] = matAlloc<FullScreenImage>();
    materials_[MaterialType::MT_SKYBOX] = matAlloc<Skybox>();
    materials_[MaterialType::MT_EQUIREC_TO_CUBE] = matAlloc<EquirecToCube>();
    materials_[MaterialType::MT_PREFILTER_ENV] = matAlloc<PrefilterEnv>();
    materials_[MaterialType::MT_LUT_GEN] = matAlloc<LutGen>();
    
    for (int32_t i = 0; i < MaterialType::MT_MAX; ++i)
    {
        materials_[i]->init();
    }
}

int32_t Renderer::addTex()
{
    LEEP_CORE_ASSERT(tex_count_ < kMaxTextures, "Max textures");
    new(textures_ + tex_count_) InternalTexture(); // Only on debug?
    return tex_count_++;
}

int32_t Renderer::addBuf()
{
    LEEP_CORE_ASSERT(buf_count_ < kMaxBuffers, "Max buffers");
    new(buffers_ + buf_count_) InternalBuffer();
    return buf_count_++;
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
    curr_pool_ = (int8_t*)m->persistentAlloc(kRenderPoolSize);
    next_pool_ = (int8_t*)m->persistentAlloc(kRenderPoolSize);
    next_offset_ = next_pool_;

    curr_queue_ = (DLComm**)m->persistentAlloc(kRenderQueueMaxCount * sizeof(int*));
    next_queue_ = (DLComm**)m->persistentAlloc(kRenderQueueMaxCount * sizeof(int*));
    curr_count_ = 0;
    next_count_ = 0;
}

void RenderQueues::addDL(DisplayList *dl)
{
    LEEP_CORE_ASSERT(next_count_ + dl->commandListCount() < 
        (int32_t)kRenderQueueMaxCount, "Max queue count reached");
    for (int32_t i = 0; i < dl->commandListCount(); ++i)
    {
        next_queue_[next_count_++] = dl->command_list()[i];
    }
}

void RenderQueues::swapQueues()
{
    int8_t *tmp = curr_pool_;
    curr_pool_ = next_pool_;
    next_pool_ = tmp;
    next_offset_ = next_pool_;

    DLComm **tmp2 = curr_queue_;
    curr_queue_ = next_queue_;
    curr_count_ = next_count_;
    next_queue_ = tmp2;
    next_count_ = 0;
}

} // namespace leep
