// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_RENDERER_H__
#define __LEEP_RENDER_RENDERER_H__ 1

#include "render/geometry.h"
#include "render/display-list.h"
#include "render/internal-resources.h"
#include "render/material-types.h"

#include <stdint.h>
#include <vector>
#include <list>
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

        std::list<int32_t> aviable_tex_pos_;
        std::vector<InternalTexture> textures_;

		std::list<int32_t> aviable_fb_pos_;
		std::vector<InternalFramebuffer> framebuffers_;

        std::list<uint32_t> aviable_buffer_pos_;
        std::vector<InternalBuffer> buffers_;

        std::unique_ptr<InternalMaterial> materials_[MaterialType::MT_MAX];

        std::list<DisplayList> next_frame_command_queue_;
        std::list<DisplayList> current_frame_commands_;

        std::mutex next_frame_command_queue_mtx_;
    };
}

#endif // __LEEP_RENDER_RENDERER_H__
