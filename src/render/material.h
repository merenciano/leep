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

        Material& operator=(const Material &other);

        void set_type(MaterialType type);
        void set_data(const PbrData &data);
        void set_data(const PlainColorData &data);

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
