// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_MATERIALS_SKYBOX_H__
#define __LEEP_RENDER_MATERIALS_SKYBOX_H__ 1

#include "render/internal-resources.h"

namespace leep
{
    class Skybox : public InternalMaterial
    {
    public:
        Skybox();
        ~Skybox();

        virtual void init() override;
        virtual void useMaterialData(const Material &material) const override;
        virtual uint32_t internal_id() const override;
    };
}

#endif // __LEEP_RENDER_MATERIALS_SKYBOX_H__