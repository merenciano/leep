#ifndef __LEEP_RENDER_INTERNAL_TEXTURE_H__
#define __LEEP_RENDER_INTERNAL_TEXTURE_H__ 1

#include <string>

namespace leep
{
    struct InternalTexture
    {
        int32_t id_ = -1;
        uint32_t texture_unit_ = 0;
        int32_t version_ = 0;
        std::string path_ = "";
        uint32_t width_;
        uint32_t height_;
        float tiling_x_;
        float tiling_y_;
    };
}

#endif // __LEEP_RENDER_INTERNAL_TEXTURE_H__