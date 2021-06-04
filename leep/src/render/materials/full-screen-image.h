// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_MATERIALS_FULL_SCREEN_IMAGE_H__
#define __LEEP_MATERIALS_FULL_SCREEN_IMAGE_H__ 1

#include "render/internal-resources.h"

namespace leep
{
    class FullScreenImage : public InternalMaterial
    {
    public:
        FullScreenImage();
        FullScreenImage(const FullScreenImage&) = delete;
        FullScreenImage(FullScreenImage&&) = delete;
        ~FullScreenImage();

        virtual void init() override;
        virtual void useMaterialData(const Material &material) const override;
        virtual uint32_t internal_id() const override;
    };
}

#endif // __LEEP_MATERIALS_FULL_SCREEN_IMAGE_H__