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
            glm::mat4 model_;
            PbrData pbr_;
            EquirecToCubeData etc_;
        };

        Material();
        Material(const Material &other);
        Material(Material&&) = delete;
        ~Material();

        Material& operator=(const Material &other);

        void set_type(MaterialType type);
        void set_model(const glm::mat4 &model);
        void set_data(const PbrData &data);
        void set_data(const EquirecToCubeData &data);
        void set_albedo(Texture tex);
        void set_metallic(Texture tex);
        void set_roughness(Texture tex);
        void set_normal(Texture tex);
        const MaterialData& data() const;
        const MaterialType type() const;
        Texture albedo() const;
        Texture metallic() const;
        Texture roughness() const;
        Texture normal() const;

    private:
        MaterialData data_; 
        MaterialType type_; // Acts as a handle
        Texture albedo_;
        Texture metallic_;
        Texture roughness_;
        Texture normal_;
    };
}
#endif // __LEEP_RENDER_MATERIAL_H__
