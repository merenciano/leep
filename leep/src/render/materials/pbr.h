// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_MATERIALS_PBR_H__
#define __LEEP_RENDER_MATERIALS_PBR_H__ 1

#include "render/internal-resources.h"

namespace leep
{
    class Pbr : public InternalMaterial
    {
    public:
        Pbr();
        ~Pbr();

        virtual void init() override;
        virtual void useMaterialData(const Material &material) const override;
        virtual uint32_t internal_id() const override;
    };
}

#endif // __LEEP_RENDER_MATERIALS_PBR_H__