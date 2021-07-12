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
    count_ = 0;
}

DrawVoxels::~DrawVoxels()
{
}

DrawVoxels &DrawVoxels::set_offset_buffer(Buffer buf, int32_t count)
{
    vxl_offsets_ = buf;
    count_ = count;

    return *this;
}

void DrawVoxels::executeSystem() const
{
    DisplayList dl;
    Material mat;
    VoxMatData data;
    data.color_ = glm::vec3(1.0f, 0.0f, 0.0f);
    data.roughness_ = 1.0f;
    data.metallic_ = 0.0f;
    mat.set_type(MaterialType::MT_VOXMAT);
    mat.set_data((float*)&data, sizeof(VoxMatData) / sizeof(float));

    dl.addCommand<Draw>()
        .set_geometry(Renderer::s_voxel)
        .set_material(mat)
        .set_instanced_opts(count_, vxl_offsets_);

    dl.submit();
}

} // End namespace clicktaclysm