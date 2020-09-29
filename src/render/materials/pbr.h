#ifndef __MOTORET_RENDER_MATERIALS_PBR_H__
#define __MOTORET_RENDER_MATERIALS_PBR_H__ 1

#include "render/internal-material.h"

namespace motoret
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

#endif // __MOTORET_RENDER_MATERIALS_PBR_H__