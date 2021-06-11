#ifndef __CLICKTACLYSM_DRAW_VOXELS_SYSTEM_H__
#define __CLICKTACLYSM_DRAW_VOXELS_SYSTEM_H__

#include "clicktaclysm.h"
#include "ecs/system.h"

namespace clicktaclysm
{
    class DrawVoxels : public leep::System
    {
    public:
        DrawVoxels();
        ~DrawVoxels();
        // Caller lose ownership !!
        // Could be done with a simple ptr but I want to set
        // it back to NULL since that ptr will not be used
        // by the caller afterwards so I don't want to make
        // a useless copy
        DrawVoxels &set_voxels(Voxel **vxl, int32_t count);
        virtual void executeSystem() const override;

    private:
        Voxel *vxls_;
        int32_t count_;
    };
}

#endif  // __CLICKTACLYSM_DRAW_VOXELS_SYSTEM_H__