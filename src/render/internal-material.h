#ifndef __MOTORET_RENDER_INTERNAL_MATERIAL_H__
#define __MOTORET_RENDER_INTERNAL_MATERIAL_H__ 1

#include <stdint.h>

namespace motoret
{
    enum MaterialTypes
    {
        NONE = -1,
        PBR = 0,

        MAX
    };

    struct InternalMaterial
    {
        uint32_t vertex_shader;
        uint32_t fragment_shader;
        uint32_t program;
    };
}
#endif // __MOTORET_RENDER_INTERNAL_MATERIAL_H__
