// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_MATERIAL_H__
#define __LEEP_RENDER_MATERIAL_H__ 1

#include "render/material-types.h"
#include "render/texture.h"

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
            SkyboxData skybox;
        };

        Material();
        Material(const Material &other);
        Material(Material&&) = delete;
        ~Material();

        Material& operator=(const Material &other);

        void set_type(MaterialType type);
        void set_world(const glm::mat4 &world);
        void set_data(const PbrData &data);
        void set_data(const PlainColorData &data);
        void set_data(const SkyboxData &data);
        void set_texture(Texture tex);
        const MaterialData& data() const;
        const MaterialType type() const;
        Texture texture() const;

    private:
        MaterialData data_; 
        MaterialType type_;
        Texture texture_;
    };
}
#endif // __LEEP_RENDER_MATERIAL_H__
