#include "use-material.h"
#include "core/manager.h"
#include "core/memory/memory.h"
#include "render/renderer.h"

#include <string.h> // memcpy

namespace leep {

UseMaterial::UseMaterial()
{
    fdata_ = nullptr;
    idata_ = nullptr;
}

UseMaterial::~UseMaterial()
{
    GM.memory().generalFree(fdata_);
    GM.memory().generalFree(idata_);
}

UseMaterial &UseMaterial::set_type(MaterialType type)
{
    type_ = type;
    return *this;
}

UseMaterial &UseMaterial::set_float(float *data, int32_t count)
{
    // Internally each unit will be a vec4
    int32_t offset = count & 3;
    if (offset)
    {
        count += 4 - offset;
    }
    size_t size = count * sizeof(float);
    fdata_ = (float*)GM.memory().generalAlloc(size);
    fcount_ = count;
    memcpy(fdata_, data, size);
    return *this;
}

UseMaterial &UseMaterial::set_tex(Texture *textures, int32_t count, int32_t cube_start)
{
    int32_t size = count * sizeof(int32_t);
    idata_ = (int32_t*)GM.memory().generalAlloc(size);
    icount_ = count;
    cube_start == -1 ? cube_start_ = count : cube_start_ = cube_start;
    for (int32_t i = 0; i < count; ++i)
    {
        LEEP_CHECK_RESOURCE(textures[i]);
        idata_[i] = GM.renderer().textures_[textures[i].handle()].texture_unit_;
    }
    return *this;
}
} // end namespace leep

#ifdef LEEP_OPENGL
    #include "render/commands/opengl/opengl-use-material.cpp"
#endif

#ifdef LEEP_OPENGL_ES
    #include "render/commands/opengl/opengl-use-material.cpp"
#endif
