#ifndef __LEEP_RENDER_TEXTURE_H__
#define __LEEP_RENDER_TEXTURE_H__ 1

#include "render/internal-texture.h"

namespace leep
{
    class Texture
    {
    public:
        Texture();
        Texture(const Texture &t);
        ~Texture();

        Texture& operator=(const Texture &t);
        void create(std::string path = "");
        void createEmpty(float width, float height);
        void release();
        int32_t id() const;

    private:
        int32_t id_;
    };
}

#endif // __LEEP_RENDER_TEXTURE_H__