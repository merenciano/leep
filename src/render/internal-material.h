#ifndef __MOTORET_RENDER_INTERNAL_MATERIAL_H__
#define __MOTORET_RENDER_INTERNAL_MATERIAL_H__ 1

#include "render/material.h"

#include <stdint.h>

namespace motoret
{
    class InternalMaterial
    {
    public:
        virtual ~InternalMaterial() {}
        virtual uint32_t internal_id() const = 0;

    protected:
        virtual void init() = 0;
        virtual void useMaterialData(Material material) const = 0;

        uint32_t internal_id_;
    };
}
#endif // __MOTORET_RENDER_INTERNAL_MATERIAL_H__
