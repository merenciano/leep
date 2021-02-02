#include "texture.h"

#include "core/common-defs.h"
#include "core/manager.h"
#include "core/window.h"
#include "render/renderer.h"

namespace leep
{
    Texture::Texture()
    {
        id_ = -1;
    }

    Texture::Texture(const Texture &t)
    {
        id_ = t.id_;
    }

    Texture::~Texture()
    {

    }

    Texture& Texture::operator=(const Texture &t)
    {
        id_ = t.id_;
        return *this;
    }

    void Texture::create(std::string path)
    {
        Renderer &r = GM.renderer();
        // texture mutex??

        if (!r.aviable_tex_pos_.empty())
        {
            id_ = r.aviable_tex_pos_.front();
            r.aviable_tex_pos_.pop_front();
        }
        else
        {
            InternalTexture tmp;
            r.textures_.push_back(tmp);
            id_ = r.textures_.size() - 1;
        }

        r.textures_[id_].id_ = 0;
        r.textures_[id_].path_ = path;
        r.textures_[id_].version_ = 0;
        r.textures_[id_].width_ = 0;
        r.textures_[id_].height_ = 0;
    }

    void Texture::createEmpty(float width, float height)
    {
        Renderer &r = GM.renderer();
        if (!r.aviable_tex_pos_.empty())
        {
            id_ = r.aviable_tex_pos_.front();
            r.aviable_tex_pos_.pop_front();
        }
        else 
        {
            InternalTexture tmp;
            r.textures_.push_back(tmp);
            id_ = r.textures_.size() - 1;
        }

        LEEP_CORE_ASSERT(((width > 1.0f && height > 1.0f) ||
            (width <= 1.0f && height <= 1.0f)),
            "Or absolute or relative, make a decision!");
        if (width > 1.0f)
        {
            r.textures_[id_].width_ = (uint32_t)width;
            r.textures_[id_].height_ = (uint32_t)height;
        }
        else
        {
            r.textures_[id_].width_ =
            (uint32_t)((float)GM.window().width() * width);
            r.textures_[id_].height_ =
            (uint32_t)((float)GM.window().height() * height);
        }

        r.textures_[id_].id_ = 0;
        r.textures_[id_].path_ = "";
        r.textures_[id_].version_ = 0;
    }

    void Texture::release()
    {
        if (id_ >= 0)
        {
            GM.renderer().textures_[id_].version_ = -1;
        }
    }

    int32_t Texture::id() const
    {
        return id_;
    }
}