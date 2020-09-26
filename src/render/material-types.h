#ifndef __MOTORET_RENDER_MATERIAL_TYPES_H__
#define __MOTORET_RENDER_MATERIAL_TYPES_H__ 1

namespace motoret
{
    enum MaterialType
    {
        NONE = -1,
        PBR = 0,
        PLAIN_COLOR = 1,

        MAX
    };

    struct PbrData
    {

    };

    struct PlainColorData
    {
        float r;
        float g;
        float b;
        float a;
    };
}
#endif // __MOTORET_RENDER_MATERIAL_TYPES_H__