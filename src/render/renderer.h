#ifndef __MOTORET_RENDER_RENDERER_H__
#define __MOTORET_RENDER_RENDERER_H__ 1

#include "render/internal-buffer.h"

#include <stdint.h>
#include <vector>
#include <list>

namespace motoret
{
    class Renderer
    {
    public:
        Renderer() {}
        ~Renderer() {}

        std::vector<InternalBuffer> buffers_;
        std::list<uint32_t> aviable_buffer_vector_positions_;
    };
}

#endif // __MOTORET_RENDER_RENDERER_H__
