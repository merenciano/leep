#include "material.h"

#include "core/common-defs.h"
#include "render/renderer.h"
#include "render/texture.h"

namespace leep {

Material::Material()
{
    type_ = MaterialType::MT_NONE;
    dcount_ = 0;
    tcount_ = 0;
    data_ = nullptr;
    tex_ = nullptr;
}

Material::~Material()
{

}

Material& Material::operator=(const Material &other)
{
    GM.memory().generalFree(data_);
    GM.memory().generalFree(tex_);
    set_data(other.data_, other.dcount_);
    set_tex(other.tex_, other.tcount_);
    type_ = other.type_;
    dcount_ = other.dcount_;
    tcount_ = other.tcount_;
    cube_start_ = other.cube_start_;
  
    return *this;
}

void Material::set_type(MaterialType type)
{
    type_ = type;
}

void Material::set_model(float *model)
{
    LEEP_CORE_ASSERT(data_, "Data ptr should be allocated already");
    memcpy(data_, model, 16 * sizeof(float));
}

void Material::set_data(float *data, int32_t count)
{
    // Internally each unit will be a vec4
    int32_t offset = count & 3;
    if (offset)
    {
        count += 4 - offset;
    }
    GM.memory().generalFree(data_);
    data_ = (float*)GM.memory().generalAlloc(count * sizeof(float));
    dcount_ = count;
    memcpy(data_, data, count * sizeof(float));
}

void Material::set_tex(Texture *tex, int32_t count, int32_t cube_start)
{
    GM.memory().generalFree(tex_);
    tex_ = (Texture*)GM.memory().generalAlloc(count * sizeof(Texture));
    tcount_ = count;
    cube_start == -1 ? cube_start_ = count : cube_start_ = cube_start;
    memcpy(tex_, tex, count * sizeof(Texture));
}

MaterialType Material::type() const
{
    return type_;
}

const float *Material::data() const
{
    return data_;
}

const Texture *Material::tex() const
{
    return tex_;
}

int32_t Material::dcount() const
{
    return dcount_;
}

int32_t Material::tcount() const
{
    return tcount_;
}

int32_t Material::cube_start() const
{
    return cube_start_;
}

} // namespace leep
