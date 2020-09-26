#ifndef __MOTORET_RENDER_INTERNAL_MATERIAL_H__
#define __MOTORET_RENDER_INTERNAL_MATERIAL_H__ 1

#include <stdint.h>

namespace motoret
{
    class InternalMaterial
    {
    public:
        virtual ~InternalMaterial() {}
        virtual void init() = 0;
        virtual uint32_t internal_id() const = 0;
    protected:
        uint32_t internal_id_;
    };
}
#endif // __MOTORET_RENDER_INTERNAL_MATERIAL_H__
