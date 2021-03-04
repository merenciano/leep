#include "delete-released.h"

#include "core/manager.h"
#include "render/renderer.h"

namespace leep {
void DeleteReleased::executeSystem() const
{
    Renderer &r = GM.renderer();
    for (int32_t i = 0; i < r.tex_count_; ++i)
    {
        if (r.textures_[i].gpu_version_ == CommonDefs::DELETED_GPU_RESOURCE)
        {
            LEEP_CORE_ASSERT(r.tex_to_del_.load() == -1,"Tex to del not ready");
            r.textures_[i].gpu_version_ = 0;
            r.textures_[i].cpu_version_ = 0;
            r.textures_[i].internal_id_ = CommonDefs::UNINIT_INTERNAL_ID;
            r.tex_to_del_.store(i);
            r.aviable_tex_pos_.emplace_front(i);
            break;
        }
    }

    for (int32_t i = 0; i < r.buf_count_; ++i)
    {
        if (r.buffers_[i].gpu_version_ == CommonDefs::DELETED_GPU_RESOURCE)
        {
            LEEP_CORE_ASSERT(r.buf_to_del_.load() == -1,"Buf to del not ready");
            r.buffers_[i].gpu_version_ = 0;
            r.buffers_[i].cpu_version_ = 0;
            r.buffers_[i].internal_id_ = CommonDefs::UNINIT_INTERNAL_ID;
            r.buf_to_del_.store(i);
            r.aviable_buffer_pos_.emplace_front(i);
            break;
        }
        ++i;
    }
}
} // namespace leep
