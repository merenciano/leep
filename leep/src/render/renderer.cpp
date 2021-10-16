#include "renderer.h"

#include "core/memory/memory.h"
#include "render/display-list.h"
#include "render/display-list-command.h"
#include "render/commands/draw.h"

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

void Renderer::init(int32_t queue_capacity)
{
    rq_.init(queue_capacity);
    textures_ = (InternalTexture*)GM.memory().persistentAlloc(sizeof(InternalTexture)*kMaxTextures);
    buffers_ = (InternalBuffer*)GM.memory().persistentAlloc(sizeof(InternalBuffer)*kMaxBuffers);
    materials_ = (InternalMaterial*)GM.memory().persistentAlloc(sizeof(InternalMaterial)*kMaxMaterials);
    gpu_materials_ = (GPUMaterial*)GM.memory().persistentAlloc(MT_MAX * sizeof(GPUMaterial));
    buf_count_ = 0;
    tex_count_ = 0;

    gpu_materials_[MT_FULL_SCREEN_IMAGE].init("fullscreen-img");
    gpu_materials_[MT_SKYBOX].init("skybox");
    gpu_materials_[MT_EQUIREC_TO_CUBE].init("eqr-to-cube");
    gpu_materials_[MT_PREFILTER_ENV].init("prefilter-env");
    gpu_materials_[MT_LUT_GEN].init("lut-gen");
    gpu_materials_[MT_PBR].init("pbr");
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

int32_t Renderer::addMat()
{
    LEEP_CORE_ASSERT(mat_count_ < kMaxMaterials, "Max materials");
    new(materials_ + mat_count_) InternalMaterial();
    return mat_count_++;
}

void Renderer::renderFrame()
{
    for (int32_t i = 0; i < rq_.curr_count_; ++i)
    {
        rq_.curr_queue_[i]->executeCommand();
        // Prevent memory leaks since render queue swaps 
        // are not calling destructors since each frame override the memory
        // so calling the commands destructor explicitly frees the
        // material data pointers on the commands that have materials
        rq_.curr_queue_[i]->~DLComm();
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

    render_pool_size_ = 0;
    render_quque_max_ = 0;
}

RenderQueues::~RenderQueues()
{
    LEEP_CORE_ERROR("RenderQueues destructor called");
}

void RenderQueues::init(int32_t capacity)
{
    LEEP_CORE_ASSERT(curr_pool_ == nullptr,
        "Already initialized.");
    int32_t pool_size = capacity * sizeof(Draw);
    curr_pool_ = (int8_t*)GM.memory().persistentAlloc(pool_size);
    next_pool_ = (int8_t*)GM.memory().persistentAlloc(pool_size);
    next_offset_ = next_pool_;

    curr_queue_ = (DLComm**)GM.memory().persistentAlloc(capacity * sizeof(void*));
    next_queue_ = (DLComm**)GM.memory().persistentAlloc(capacity * sizeof(void*));
    curr_count_ = 0;
    next_count_ = 0;

    render_pool_size_ = pool_size;
    render_quque_max_ = capacity;
}

void RenderQueues::addDL(DisplayList *dl)
{
    LEEP_CORE_ASSERT(next_count_ + dl->commandListCount() < 
        render_quque_max_, "Max queue count reached");
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
