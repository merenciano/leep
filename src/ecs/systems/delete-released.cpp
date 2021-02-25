#include "delete-released.h"

#include "core/manager.h"
#include "render/renderer.h"

namespace leep {
void DeleteReleased::executeSystem() const
{
    Renderer &r = GM.renderer();
    int32_t i = 0;
    for (InternalTexture &t : r.textures_)
    {
        if (t.gpu_version_ == CommonDefs::DELETED_GPU_RESOURCE)
        {
            LEEP_CORE_ASSERT(r.tex_to_del_.load() == -1,"Tex to del not ready");
            t.gpu_version_ = 0;
            t.cpu_version_ = 0;
            t.internal_id_ = CommonDefs::UNINIT_INTERNAL_ID;
            r.tex_to_del_.store(i);
            r.aviable_tex_pos_.emplace_front(i);
            break;
        }
        ++i;
    }

    i = 0;
    for (InternalBuffer &b : r.buffers_)
    {
        if (b.gpu_version_ == CommonDefs::DELETED_GPU_RESOURCE)
        {
            LEEP_CORE_ASSERT(r.buf_to_del_.load() == -1,"Buf to del not ready");
            b.gpu_version_ = 0;
            b.cpu_version_ = 0;
            b.internal_id_ = CommonDefs::UNINIT_INTERNAL_ID;
            r.buf_to_del_.store(i);
            r.aviable_buffer_pos_.emplace_front(i);
            break;
        }
        ++i;
    }
}
} // namespace leep
