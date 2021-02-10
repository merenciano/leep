#include "texture.h"

#include "core/common-defs.h"
#include "core/manager.h"
#include "core/window.h"
#include "render/renderer.h"

namespace leep
{
    Texture::Texture()
    {
        handler_ = ConstantValues::UNINITIALIZED_HANDLER;
    }

    Texture::Texture(const Texture &t)
    {
        handler_ = t.handler_;
    }

    Texture::~Texture()
    {

    }

    Texture& Texture::operator=(const Texture &t)
    {
        handler_ = t.handler_;
        return *this;
    }

    void Texture::create(std::string path, bool cube)
    {
        LEEP_ASSERT(path != "" || cube != true, "The cubemap needs a path to a directory");
		LEEP_ASSERT(handler_ == ConstantValues::UNINITIALIZED_HANDLER, "This texture has been created before");
        Renderer &r = GM.renderer();
        // texture mutex??

        if (!r.aviable_tex_pos_.empty())
        {
            handler_ = r.aviable_tex_pos_.front();
            r.aviable_tex_pos_.pop_front();
        }
        else
        {
            InternalTexture tmp;
            r.textures_.push_back(tmp);
            handler_ = r.textures_.size() - 1;
        }

        r.textures_[handler_].internal_id_ = 0;
        r.textures_[handler_].path_ = path;
        r.textures_[handler_].version_ = 0;
        r.textures_[handler_].width_ = 0;
        r.textures_[handler_].height_ = 0;
        r.textures_[handler_].cube_ = cube;
    }

    void Texture::createEmpty(float width, float height)
    {
        LEEP_ASSERT(handler_ == ConstantValues::UNINITIALIZED_HANDLER, "This texture has been created before");
        Renderer &r = GM.renderer();
        if (!r.aviable_tex_pos_.empty())
        {
            handler_ = r.aviable_tex_pos_.front();
            r.aviable_tex_pos_.pop_front();
        }
        else 
        {
            InternalTexture tmp;
            r.textures_.push_back(tmp);
            handler_ = r.textures_.size() - 1;
        }

        LEEP_CORE_ASSERT(((width > 1.0f && height > 1.0f) ||
            (width <= 1.0f && height <= 1.0f)),
            "Or absolute or relative, make a decision!");
        if (width > 1.0f)
        {
            r.textures_[handler_].width_ = (uint32_t)width;
            r.textures_[handler_].height_ = (uint32_t)height;
        }
        else
        {
            r.textures_[handler_].width_ =
            (uint32_t)((float)GM.window().width() * width);
            r.textures_[handler_].height_ =
            (uint32_t)((float)GM.window().height() * height);
        }

        r.textures_[handler_].internal_id_ = 0;
        r.textures_[handler_].path_ = "";
        r.textures_[handler_].version_ = 0;
		r.textures_[handler_].cube_ = false; // TODO: Create empty cube tex if I add pointlight shadows
    }

    void Texture::release()
    {
        if (handler_ >= 0)
        {
            GM.renderer().textures_[handler_].version_ = -1;
			handler_ = ConstantValues::DELETED_HANDLER;
        }
    }

    int32_t Texture::id() const
    {
        return handler_;
    }
}