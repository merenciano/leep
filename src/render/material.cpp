#include "render/material.h"

#include "core/common-defs.h"

namespace leep
{
    Material::Material()
    {

    }

    Material::~Material()
    {

    }

    Material& Material::operator=(const Material &other)
    {
        LEEP_CORE_ASSERT(other.type_ != MaterialType::MT_NONE, "You are trying to copy an uninitialized material");
        type_ = other.type_;
        switch(type_)
        {
        case MaterialType::MT_PBR:
            data_.pbr = other.data_.pbr;
            break;
        
        case MaterialType::MT_PLAIN_COLOR:
            data_.plain_color = other.data_.plain_color;
            break;

        default:
            LEEP_CORE_ERROR("Operator '=' of material default case.");
            break;
        }
        return *this;
    }

    void Material::set_type(MaterialType type)
    {
        type_ = type;
    }

    void Material::set_world(const glm::mat4 &world)
    {
        data_.world = world;    
    }

    void Material::set_data(const PbrData &data)
    {
        LEEP_CORE_ASSERT(type_ == MaterialType::MT_PBR, "The material has not the correct type yet.");
        data_.pbr = data;
    }

    void Material::set_data(const PlainColorData &data)
    {
        LEEP_CORE_ASSERT(type_ == MaterialType::MT_PLAIN_COLOR, "The material has not the correct type yet.");
        data_.plain_color = data;
    }

    const Material::MaterialData& Material::data() const
    {
        return data_;
    }

    const MaterialType Material::type() const
    {
        return type_;
    }
}