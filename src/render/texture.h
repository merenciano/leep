#ifndef __LEEP_RENDER_TEXTURE_H__
#define __LEEP_RENDER_TEXTURE_H__ 1

#include <string>

namespace leep
{
    enum class TexType
    {
        R,
        RGB,
        SRGB,
        FLOAT16,
        CUBE,
        DEPTH,
    };
    class Texture
    {
    public:
        Texture();
        Texture(const Texture &t);
        ~Texture();

        Texture& operator=(const Texture &t);
        void create(std::string path = "", TexType t = TexType::SRGB);
        void createEmpty(float width, float height, TexType t = TexType::FLOAT16);
        void release();
        int32_t handle() const;

    private:
        int32_t handle_;
    };
}

#endif // __LEEP_RENDER_TEXTURE_H__