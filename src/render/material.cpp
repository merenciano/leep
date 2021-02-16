#include "material.h"

#include "core/common-defs.h"

namespace leep
{
    Material::Material()
    {
        type_ = MaterialType::MT_NONE;
    }

    Material::~Material()
    {

    }

    Material::Material(const Material &other)
    {
        type_ = other.type_;
        albedo_ = other.albedo_;
        metallic_ = other.metallic_;
        roughness_ = other.roughness_;
        normal_ = other.normal_;

        switch(type_)
        {
        case MaterialType::MT_NONE:
            LEEP_CORE_WARNING("You are copying an uninitialized material");
            break;

        case MaterialType::MT_PBR:
            data_.pbr_ = other.data_.pbr_;
            break;
        
        case MaterialType::MT_FULL_SCREEN_IMAGE:
            break;

        default:
            LEEP_CORE_WARNING("Copy constructor of material default case.");
            break;
        }
    }

    Material& Material::operator=(const Material &other)
    {
        //LEEP_CORE_ASSERT(other.type_ != MaterialType::MT_NONE, "You are trying to copy an uninitialized material");
        type_ = other.type_;
        albedo_ = other.albedo_;
        metallic_ = other.metallic_;
        roughness_ = other.roughness_;
        normal_ = other.normal_;

        switch(type_)
        {
        case MaterialType::MT_PBR:
            data_.pbr_ = other.data_.pbr_;
            break;
        
        case MaterialType::MT_FULL_SCREEN_IMAGE:
            break;

        default:
            //LEEP_CORE_ERROR("Operator '=' of material default case.");
            break;
        }
        return *this;
    }

    void Material::set_type(MaterialType type)
    {
        type_ = type;
    }

    void Material::set_model(const glm::mat4 &world)
    {
        data_.model_ = world;    
    }

    void Material::set_data(const PbrData &data)
    {
        LEEP_CORE_ASSERT(type_ == MaterialType::MT_PBR, "The material has not the correct type yet.");
        data_.pbr_ = data;
    }

    void Material::set_albedo(Texture texture)
    {
        albedo_ = texture;
    }

    void Material::set_metallic(Texture texture)
    {
        metallic_ = texture;
    }

    void Material::set_roughness(Texture texture)
    {
        roughness_ = texture;
    }

    void Material::set_normal(Texture texture)
    {
        normal_ = texture;
    }

    const Material::MaterialData& Material::data() const
    {
        return data_;
    }

    const MaterialType Material::type() const
    {
        return type_;
    }

    Texture Material::albedo() const
    {
        return albedo_;
    }

    Texture Material::metallic() const
    {
        return metallic_;
    }

    Texture Material::roughness() const
    {
        return roughness_;
    }

    Texture Material::normal() const
    {
        return normal_;
    }
}