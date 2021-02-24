#ifndef __LEEP_RENDER_TEXTURE_H__
#define __LEEP_RENDER_TEXTURE_H__ 1

#include <string>

namespace leep
{
    enum class TexType
    {
        NONE,
        R,
        RGB,
        SRGB,
        DEPTH,
        SKYBOX,
        RGB_F16,
        RGBA_F16,
        ENVIRONMENT,
        PREFILTER_ENVIRONMENT,
    };
    class Texture
    {
    public:
        Texture();
        Texture(const Texture &t);
        ~Texture();

        Texture& operator=(const Texture &t);
        void create(std::string path, TexType t);
        void createEmpty(float width, float height, TexType t);
        void release();
        int32_t handle() const;

    private:
        int32_t handle_;
    };
}

#endif // __LEEP_RENDER_TEXTURE_H__