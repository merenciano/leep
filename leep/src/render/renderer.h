// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_RENDERER_H__
#define __LEEP_RENDER_RENDERER_H__

#include "core/memory/buddy-alloc-stl.h"
#include "render/geometry.h"
#include "render/internal-resources.h"
#include "render/material-types.h"

#include <stdint.h>
#include <forward_list>
#include <vector>
#include <atomic>
#include <mutex>

namespace leep
{
    const size_t kFrameMemorySize = 1024 * 64;
    class DLComm;
    class DisplayList;

    class RenderQueues
    {
    public:
        RenderQueues();
        RenderQueues(const RenderQueues&) = delete;
        RenderQueues(RenderQueues&&) = delete;
        ~RenderQueues();

        void init(int32_t capacity);
        void addDL(DisplayList *dl);
        void swapQueues();

        template<typename T>
        DLComm *commandAlloc();

        DLComm **curr_queue_;
        DLComm **next_queue_;

        int32_t next_count_;
        int32_t curr_count_;
        int8_t *curr_pool_; // Main thread renders this
        int8_t *next_pool_; // Logic thread fill this for the next frame
        int8_t *next_offset_;

        // TODO: This should be private with getters only
        int32_t render_pool_size_;
        int32_t render_quque_max_;
    };

    class Renderer
    {
    public:
        static Geometry s_cube;
        static Geometry s_voxel;
        static Geometry s_sphere;
        static Geometry s_quad;

        Renderer();
        Renderer(const Renderer&) = delete;
        Renderer(Renderer&&) = delete;
        ~Renderer();

        void init(int32_t queue_capacity);

        void renderFrame();
        void submitFrame();
        void deleteResources();

        glm::vec4 sun_dir_intensity_;

        // Forward list because random element access is not needed
        // and I only need push and pop front which are cheap in this container
        std::forward_list<int32_t, BuddySTL<int32_t>> aviable_buffer_pos_;
        std::forward_list<int32_t, BuddySTL<int32_t>> aviable_tex_pos_;
        std::forward_list<int32_t, BuddySTL<int32_t>> aviable_fb_pos_; // TODO: remove framebuffer creation and deletion

        std::atomic<int32_t> tex_to_del_;
        std::atomic<int32_t> buf_to_del_;

        int32_t addTex();
        int32_t addBuf();
        int32_t addMat();

        // TODO: Move the camera here
        InternalFramebuffer framebuffer_;
        std::vector<InternalFramebuffer,
                    BuddySTL<InternalFramebuffer>> framebuffers_;

        GPUMaterial *gpu_materials_;
        InternalBuffer *buffers_;
        InternalTexture *textures_;
        InternalMaterial *materials_;
        int32_t buf_count_;
        int32_t tex_count_;
        int32_t mat_count_;

        std::mutex mtx_;

        RenderQueues rq_;

        void *allocFrameData(size_t size);

    private:
        uint8_t frame_mem_render_; // switch, each frame changes between 0 and 1. It indicates which buffer has the render data
        uint8_t *frame_mem_[2];
        uint8_t *frame_mem_last_;
    };

    template<typename T>
    DLComm *RenderQueues::commandAlloc()
    {
        // Here I assume that memory will always be aligned to 8 because
        // the all the commands have vtable (executeCommand virtual method)
        // so the class is aligned to 8 (sizeof(T) will always be multiple of 8)
        LEEP_CORE_ASSERT(((size_t)next_offset_ & (size_t)7) == 0, "Mem not aligned to 8");
        DLComm *mem = new(next_offset_) T();
        next_offset_ += sizeof(T);
        LEEP_CORE_ASSERT(next_offset_ - next_pool_ < render_pool_size_,
            "Queue doesn't fit in the chunk");
        return mem;
    }
}

#endif // __LEEP_RENDER_RENDERER_H__
