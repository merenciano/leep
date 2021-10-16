// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_MATERIAL_H__
#define __LEEP_RENDER_MATERIAL_H__

#include "render/material-types.h"
#include "render/texture.h"

namespace leep
{
    class Texture;

    class Material
    {
    public:
        Material();
        ~Material();

        Material(const Material&) = default;
        Material(Material&&) = delete;
        Material &operator=(const Material &other) = default;

        void create(MaterialType type, float *data, int32_t dcount,
                    Texture *tex, int32_t tcount, int32_t cube_start = -1);

        void set_model(const float *model) const;

        MaterialType type() const;
        const float *data() const;
        const Texture *tex() const;
        int32_t dcount() const;
        int32_t tcount() const;
        int32_t cube_start() const;

    private:
        int32_t handle_;
    };
}
#endif // __LEEP_RENDER_MATERIAL_H__
