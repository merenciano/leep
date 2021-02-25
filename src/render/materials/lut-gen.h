/**
 * @file lut-gen.h
 * @author Lucas Merenciano Martinez (lucasmermar@gmail.com)
 * @brief Material used for the brdf lut texture generation
 * @version 0.1
 */

#ifndef __LEEP_MATERIALS_LUT_GENERATOR_H__
#define __LEEP_MATERIALS_LUT_GENERATOR_H__

#include "render/internal-resources.h"

namespace leep
{
    class LutGen : public InternalMaterial
    {
    public:
        LutGen();
        LutGen(const LutGen&) = delete;
        LutGen(LutGen&&) = delete;
        ~LutGen();

        virtual void init() override;
        virtual void useMaterialData(const Material &material) const override;
        virtual uint32_t internal_id() const override;
    };
}

#endif // __LEEP_MATERIALS_LUT_GENERATOR_H__