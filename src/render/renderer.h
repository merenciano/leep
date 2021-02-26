// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_RENDERER_H__
#define __LEEP_RENDER_RENDERER_H__

#include "core/memory/memory.h"
#include "render/geometry.h"
#include "render/internal-resources.h"
#include "render/material-types.h"

#include <stdint.h>
#include <deque>
#include <forward_list>
#include <memory>
#include <mutex>
#include <atomic>

namespace leep
{
    class Memory;
    class DLComm;
    class DisplayList;

    class RenderQueues
    {
    public:
        RenderQueues();
        RenderQueues(const RenderQueues&) = delete;
        RenderQueues(RenderQueues&&) = delete;
        ~RenderQueues();

        void init(Memory *m);
        void addDL(DisplayList *dl);

        template<typename T>
        DLComm *commandAlloc()
        {
            // Here I assume that memory will always be aligned to 8 because
            // the all the commands have vtable (executeCommand virtual meethod)
            // so the class is aligned to 8 (sizeof(T) will always be multiple of 8)
            LEEP_CORE_ASSERT(((size_t)next_offset_ & (size_t)7) == 0, "Mem not aligned to 8");
            DLComm *mem = new(next_offset_) T();
            next_offset_ += sizeof(T);
            LEEP_CORE_ASSERT(next_offset_ - next_pool_ < kRenderPoolSize,
                "Queue doesn't fit in the chunk");
            return mem;
        }


        void swapQueues()
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

        int8_t *curr_pool_; // Main thread renders this
        int8_t *next_pool_; // Logic thread fill this for the next frame
        int8_t *next_offset_;
        DLComm **curr_queue_;
        DLComm **next_queue_;
        int16_t curr_count_;
        int16_t next_count_;
    };

    class Renderer
    {
    public:
        static Geometry s_cube;
        static Geometry s_sphere;
        static Geometry s_quad;

        Renderer();
        Renderer(const Renderer&) = delete;
        Renderer(Renderer&&) = delete;
        ~Renderer();

        void init();

        void renderFrame();
        void submitFrame();
        void deleteResources();

        // Forward list because random element access is not needed
        // and I only need push and pop front which are cheap in this container
        std::forward_list<int32_t> aviable_buffer_pos_;
        std::forward_list<int32_t> aviable_tex_pos_;
        std::forward_list<int32_t> aviable_fb_pos_; // TODO: remove framebuffer creation and deletion

        // Here I need random element access so I decided deque over vector
        // because of the vector reallocations
        std::deque<InternalBuffer> buffers_;
        std::deque<InternalTexture> textures_;
        std::deque<InternalFramebuffer> framebuffers_; // TODO: Change this to fixed size array

        // Constant size
        // TODO: permanent allocator
        std::unique_ptr<InternalMaterial> materials_[MaterialType::MT_MAX];

        std::atomic<int32_t> tex_to_del_;
        std::atomic<int32_t> buf_to_del_;

        //InternalBuffer *buffers_;
        //InternalTexture *textures_;
        RenderQueues rq_;
    };
}

#endif // __LEEP_RENDER_RENDERER_H__
