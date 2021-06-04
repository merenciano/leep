/**
 * @file prefilter-env.h
 * @author Lucas Merenciano Martinez (lucasmermar@gmail.com)
 * @brief Pre-filters environment cube map using GGX NDF importance sampling. 
 * Part of specular IBL split-sum approximation.
 * @version 0.1
 * 
 */

#ifndef __LEEP_RENDER_MATERIALS_PREFILTER_ENV_H__
#define __LEEP_RENDER_MATERIALS_PREFILTER_ENV_H__

#include "render/internal-resources.h"

namespace leep
{
    class PrefilterEnv : public InternalMaterial
    {
    public:
        PrefilterEnv();
        ~PrefilterEnv();

        virtual void init() override;
        virtual void useMaterialData(const Material &material) const override;
        virtual uint32_t internal_id() const override;
    };
}

#endif // __LEEP_RENDER_MATERIALS_PREFILTER_ENV_H__