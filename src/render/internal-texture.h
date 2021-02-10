#ifndef __LEEP_RENDER_INTERNAL_TEXTURE_H__
#define __LEEP_RENDER_INTERNAL_TEXTURE_H__ 1

#include <string>

namespace leep
{
    struct InternalTexture
    {
        int32_t internal_id_ = -1;
        uint32_t texture_unit_ = 0;
        int32_t version_ = 0;
        uint32_t width_;
        uint32_t height_;
        std::string path_ = "";
        bool cube_;
    };
}

#endif // __LEEP_RENDER_INTERNAL_TEXTURE_H__