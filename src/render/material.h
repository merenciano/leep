#ifndef __MOTORET_RENDER_MATERIAL_H__
#define __MOTORET_RENDER_MATERIAL_H__ 1

#include "render/internal-material.h"
#include "render/material-types.h"

namespace motoret
{
    class Material
    {
    public:
        Material();
        Material(const Material&) = delete;
        Material(Material&&) = delete;
        ~Material();

        void set_params(PbrData);
        void set_params(PlainColorData);

        MaterialType type();

    protected:
        union MaterialData
        {
            PbrData pbr;
            PlainColorData plain_color;
        } data_;
        
        MaterialType type_;
    };
}
#endif // __MOTORET_RENDER_MATERIAL_H__
