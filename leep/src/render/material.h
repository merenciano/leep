// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_MATERIAL_H__
#define __LEEP_RENDER_MATERIAL_H__

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
            PrefilterEnvData pref_;
        };

        Material();
        Material(const Material &other);
        Material(Material&&) = delete;
        ~Material();

        Material& operator=(const Material &other);

        void set_type(MaterialType type);
        void set_model(float *model);
        void set_data(float *data, int32_t count);
        void set_tex(class Texture *tex, int32_t count, int32_t cube_start = -1);

        MaterialType type() const;
        const float *data() const;
        const Texture *tex() const;
        int32_t dcount() const;
        int32_t tcount() const;
        int32_t cube_start() const;

    private:
        // TODO: Check if I can merge this with use-material command data
        // since they are pretty similar
        float *data_; 
        Texture *tex_;
        MaterialType type_; // Acts as a handle
        int32_t dcount_;
        int32_t tcount_;
        int32_t cube_start_;
    };
}
#endif // __LEEP_RENDER_MATERIAL_H__
