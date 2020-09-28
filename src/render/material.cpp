#include "render/material.h"

#include "core/common-defs.h"

namespace motoret
{
    Material::Material()
    {

    }

    Material::~Material()
    {

    }

    Material& Material::operator=(const Material &other)
    {
        MOTORET_CORE_ASSERT(other.type_ == MaterialType::NONE, "You are trying to copy an uninitialized material");
        type_ = other.type_;
        switch(type_)
        {
        case MaterialType::PBR:
            data_.pbr = other.data_.pbr;
            break;
        
        case MaterialType::PLAIN_COLOR:
            data_.plain_color = other.data_.plain_color;
            break;

        default:
            MOTORET_CORE_ERROR("Operator '=' of material default case.");
            break;
        }
    }

    void Material::set_type(MaterialType type)
    {
        type_ = type;
    }

    void Material::set_data(const PbrData &data)
    {
        MOTORET_CORE_ASSERT(type_ == MaterialType::PBR, "The material has not the correct type yet.");
        data_.pbr = data;
    }

    void Material::set_data(const PlainColorData &data)
    {
        MOTORET_CORE_ASSERT(type_ == MaterialType::PLAIN_COLOR, "The material has not the correct type yet.");
        data_.plain_color = data;
    }

    const Material::MaterialData& Material::data() const
    {
        return data_;
    }
}