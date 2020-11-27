// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_MATERIALS_PLAIN_COLOR_H__
#define __LEEP_MATERIALS_PLAIN_COLOR_H__ 1

#include "render/internal-material.h"

namespace leep
{
    class PlainColor : public InternalMaterial
    {
    public:
        PlainColor();
        PlainColor(const PlainColor&) = delete;
        PlainColor(PlainColor&&) = delete;
        ~PlainColor();

        virtual void init() override;
        virtual void useMaterialData(const Material &material) const override;
        virtual uint32_t internal_id() const override;
    };
}

#endif // __LEEP_MATERIALS_PLAIN_COLOR_H__