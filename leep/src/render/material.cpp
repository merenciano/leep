#include "material.h"

#include "core/common-defs.h"
#include "render/renderer.h"
#include "render/texture.h"

namespace leep {

Material::Material()
{
    handle_ = UNINIT_HANDLE;
}

Material::~Material()
{
    handle_ = DELETED_HANDLE;
}

void Material::create(MaterialType type, float *data, int32_t dcount,
                      Texture *tex, int32_t tcount, int32_t cube_start)
{
    LEEP_ASSERT(handle_ == UNINIT_HANDLE, "This material has been created already");
    handle_ = GM.renderer().addMat();
    InternalMaterial &mat = GM.renderer().materials_[handle_];
    mat.type_ = type;
    mat.data_ = data;
    mat.dcount_ = dcount;
    mat.tex_ = tex;
    mat.tcount_ = tcount;
    mat.cube_start_ = cube_start;
}

void Material::set_model(const float *model) const
{
    LEEP_ASSERT(model, "Null ptr");
    InternalMaterial &mat = GM.renderer().materials_[handle_];
    memcpy(mat.data_, model, 64); // 4x4 matrix of float
}

MaterialType Material::type() const
{
    return GM.renderer().materials_[handle_].type_;
}

const float *Material::data() const
{
    return GM.renderer().materials_[handle_].data_;
}

const Texture *Material::tex() const
{
    return GM.renderer().materials_[handle_].tex_;
}

int32_t Material::dcount() const
{
    return GM.renderer().materials_[handle_].dcount_;
}

int32_t Material::tcount() const
{
    return GM.renderer().materials_[handle_].tcount_;
}

int32_t Material::cube_start() const
{
    return GM.renderer().materials_[handle_].cube_start_;
}

} // namespace leep
