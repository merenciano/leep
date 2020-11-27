// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_MATERIAL_H__
#define __LEEP_RENDER_MATERIAL_H__ 1

#include "render/material-types.h"

namespace leep
{
    class Material
    {
    public:
        union MaterialData
        {
            glm::mat4 world;
            PbrData pbr;
            PlainColorData plain_color;
        };

        Material();
        Material(const Material&) = delete;
        Material(Material&&) = delete;
        ~Material();

        Material& operator=(const Material &other);

        void set_type(MaterialType type);
        void set_world(const glm::mat4 &world);
        void set_data(const PbrData &data);
        void set_data(const PlainColorData &data);
        const MaterialData& data() const;

        const MaterialType type() const;

    protected:
        MaterialData data_; 
        MaterialType type_;
    };
}
#endif // __LEEP_RENDER_MATERIAL_H__
