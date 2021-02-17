// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_MATERIALS_EQUIREC_TO_CUBE_H__
#define __LEEP_MATERIALS_EQUIREC_TO_CUBE_H__

#include "render/internal-resources.h"

namespace leep
{
    class EquirecToCube : public InternalMaterial
    {
    public:
        EquirecToCube();
        EquirecToCube(const EquirecToCube&) = delete;
        EquirecToCube(EquirecToCube&&) = delete;
        ~EquirecToCube();

        virtual void init() override;
        virtual void useMaterialData(const Material &material) const override;
        virtual uint32_t internal_id() const override;
    };
}

#endif // __LEEP_MATERIALS_EQUIREC_TO_CUBE_H__
