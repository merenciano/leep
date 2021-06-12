#include "draw-voxels.h"

#include "core/common-defs.h"
#include "core/manager.h"
#include "tools/resource-map.h"
#include "core/memory/memory.h"
#include "render/renderer.h"
#include "render/display-list.h"
#include "render/commands/draw.h"

namespace clicktaclysm {
using namespace leep;

DrawVoxels::DrawVoxels()
{
    vxls_ = nullptr;
    count_ = 0;
}

DrawVoxels::~DrawVoxels()
{
    GM.memory().generalFree(vxls_);
}

DrawVoxels &DrawVoxels::set_voxels(Voxel **vxl, int32_t count)
{
    vxls_ = *vxl;
    count_ = count;
    *vxl = nullptr;

    return *this;
}

void DrawVoxels::executeSystem() const
{
    if (!vxls_)
    {
        LEEP_WARNING("DrawVoxels system called without voxels setted");
        return;
    }

    DisplayList dl;
    Material mat;
    VoxMatData data;
    data.color_ = glm::vec3(1.0f, 0.0f, 0.0f);
    data.roughness_ = 1.0f;
    data.metallic_ = 0.0f;
    mat.set_type(MaterialType::MT_VOXMAT);

    for (int32_t i = 0; i < count_; ++i)
    {
        glm::mat4 model(1.0f);
        model = glm::translate(model, vxls_[i]);
        mat.set_data((float*)&data, sizeof(VoxMatData) / sizeof(float));
        mat.set_model((float*)&model);

        dl.addCommand<Draw>().set_geometry(Renderer::s_voxel).set_material(mat);
    }
    dl.submit();
}

} // End namespace clicktaclysm