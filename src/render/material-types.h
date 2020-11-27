#ifndef __MOTORET_RENDER_MATERIAL_TYPES_H__
#define __MOTORET_RENDER_MATERIAL_TYPES_H__ 1

#include "glm/mat4x4.hpp"

namespace motoret
{
    // Not enum class because its values are used as array indexes for the renderer materials
    enum MaterialType
    {
        MT_NONE = -1,
        MT_PBR = 0,
        MT_PLAIN_COLOR = 1,

        MT_MAX
    };

    struct WorldData
    {
        glm::mat4 world;
    };

    struct PbrData
    {
        glm::mat4 world;
    };

    struct PlainColorData
    {
        glm::mat4 world;
        float r;
        float g;
        float b;
        float a;
    };
}
#endif // __MOTORET_RENDER_MATERIAL_TYPES_H__