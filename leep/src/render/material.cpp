#include "material.h"

#include "core/common-defs.h"

namespace leep {

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
}

Material& Material::operator=(const Material &other)
{
    type_ = other.type_;
    albedo_ = other.albedo_;
    metallic_ = other.metallic_;
    roughness_ = other.roughness_;
    normal_ = other.normal_;
    data_ = other.data_;
  
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
    LEEP_CORE_ASSERT(type_ == MaterialType::MT_PBR,
        "The material doesn't have the correct type.");
    data_.pbr_ = data;
}

void Material::set_data(const EquirecToCubeData &data)
{
    LEEP_CORE_ASSERT(type_ == MaterialType::MT_EQUIREC_TO_CUBE,
        "The material doesn't have the correct type.");
    data_.etc_ = data;
}

void Material::set_data(const PrefilterEnvData &data)
{
    LEEP_CORE_ASSERT(type_ == MaterialType::MT_PREFILTER_ENV,
        "The material doesn't have the correct type.");
    data_.pref_ = data;
}

void Material::set_albedo(Texture t)
{
    LEEP_CHECK_RESOURCE(t);
    albedo_ = t;
}

void Material::set_metallic(Texture t)
{
    LEEP_CHECK_RESOURCE(t);
    metallic_ = t;
}

void Material::set_roughness(Texture t)
{
    LEEP_CHECK_RESOURCE(t);
    roughness_ = t;
}

void Material::set_normal(Texture t)
{
    LEEP_CHECK_RESOURCE(t);
    normal_ = t;
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

} // namespace leep
