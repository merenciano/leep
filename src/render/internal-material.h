#ifndef __MOTORET_RENDER_INTERNAL_MATERIAL_H__
#define __MOTORET_RENDER_INTERNAL_MATERIAL_H__ 1

#include <stdint.h>

namespace motoret
{
    class InternalMaterial
    {
        uint32_t vertex_shader_;
        uint32_t fragment_shader_;
        uint32_t program_;
    };
}
#endif // __MOTORET_RENDER_INTERNAL_MATERIAL_H__
