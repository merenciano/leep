// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_RENDERER_H__
#define __LEEP_RENDER_RENDERER_H__ 1

#include "render/geometry.h"
#include "render/display-list.h"
#include "render/internal-buffer.h"
#include "render/internal-material.h"
#include "render/internal-texture.h"
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

        Renderer();
        ~Renderer();

        void init();
        void addDisplayListToQueue(DisplayList &&dl);

        void renderFrame();
        void submitFrame();

        std::list<int32_t> aviable_tex_pos_;
        std::vector<InternalTexture> textures_;

        std::vector<InternalBuffer> buffers_;
        std::list<uint32_t> aviable_buffer_vector_positions_;
        std::unique_ptr<InternalMaterial> materials_[MaterialType::MT_MAX];

        std::list<DisplayList> next_frame_command_queue_;
        std::list<DisplayList> current_frame_commands_;

        std::mutex next_frame_command_queue_mtx_;
        std::mutex current_frame_commands_mtx_;
    };
}

#endif // __LEEP_RENDER_RENDERER_H__
