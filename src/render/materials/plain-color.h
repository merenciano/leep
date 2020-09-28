#ifndef __MOTORET_MATERIALS_PLAIN_COLOR_H__
#define __MOTORET_MATERIALS_PLAIN_COLOR_H__ 1

#include "render/internal-material.h"

namespace motoret
{
    class PlainColor : public InternalMaterial
    {
    public:
        PlainColor();
        PlainColor(const PlainColor&) = delete;
        PlainColor(PlainColor&&) = delete;
        ~PlainColor();

        virtual void init() override;
        virtual void useMaterialData(Material material) const override;
        virtual uint32_t internal_id() const override;

    };
}

#endif // __MOTORET_MATERIALS_PLAIN_COLOR_H__