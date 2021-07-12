#ifndef __CLICKTACLYSM_DRAW_VOXELS_SYSTEM_H__
#define __CLICKTACLYSM_DRAW_VOXELS_SYSTEM_H__

#include "clicktaclysm.h"
#include "ecs/system.h"
#include "render/buffer.h"

namespace clicktaclysm
{
    class DrawVoxels : public leep::System
    {
    public:
        DrawVoxels();
        ~DrawVoxels();
        DrawVoxels &set_offset_buffer(leep::Buffer buf, int32_t count);
        virtual void executeSystem() const override;

    private:
        leep::Buffer vxl_offsets_;
        int32_t count_;
    };
}

#endif  // __CLICKTACLYSM_DRAW_VOXELS_SYSTEM_H__