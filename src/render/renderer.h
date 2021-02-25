// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_RENDERER_H__
#define __LEEP_RENDER_RENDERER_H__ 1

#include "render/geometry.h"
#include "render/display-list.h"
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
    class Renderer
    {
    public:
        static Geometry s_cube;
        static Geometry s_sphere;
        static Geometry s_quad;

        Renderer();
        ~Renderer();

        void init();
        void addDisplayListToQueue(DisplayList &&dl);

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
        std::unique_ptr<InternalMaterial> materials_[MaterialType::MT_MAX];

        // Deque because of the vector reallocations
        // NOTE: Mutex if different threads from logic can submit display lists
        std::list<DisplayList> next_frame_command_queue_;
        std::list<DisplayList> current_frame_commands_;

        std::atomic<int32_t> tex_to_del_;
        std::atomic<int32_t> buf_to_del_;
    private:
        std::mutex nxt_frame_q_mtx_;
    };
}

#endif // __LEEP_RENDER_RENDERER_H__
