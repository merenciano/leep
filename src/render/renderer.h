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

        std::forward_list<int32_t> aviable_tex_pos_;
        std::deque<InternalTexture> textures_;

		std::forward_list<int32_t> aviable_fb_pos_;
		std::deque<InternalFramebuffer> framebuffers_;

        // Forward list because random element access is not needed
        // and I only need push and pop front which are cheap in this container
        std::forward_list<uint32_t> aviable_buffer_pos_;
        // Here I need random element access so I decided deque over vector
        // because of the vector reallocations
        std::deque<InternalBuffer> buffers_;

        // Constant size
        std::unique_ptr<InternalMaterial> materials_[MaterialType::MT_MAX];

        // Deque because of the vector reallocations
        std::deque<DisplayList> next_frame_command_queue_;
        std::deque<DisplayList> current_frame_commands_;

    private:
        std::mutex nxt_frame_q_mtx_;
    };
}

#endif // __LEEP_RENDER_RENDERER_H__
