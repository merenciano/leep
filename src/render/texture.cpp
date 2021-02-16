#include "texture.h"

#include "core/common-defs.h"
#include "core/manager.h"
#include "core/window.h"
#include "render/renderer.h"

namespace leep
{
    Texture::Texture()
    {
        handle_ = ConstantValues::UNINITIALIZED_HANDLER;
    }

    Texture::Texture(const Texture &t)
    {
        handle_ = t.handle_;
    }

    Texture::~Texture()
    {

    }

    Texture& Texture::operator=(const Texture &t)
    {
        handle_ = t.handle_;
        return *this;
    }

    void Texture::create(std::string path, bool linear, bool cube)
    {
        LEEP_ASSERT(path != "" || cube != true, "The cubemap needs a path to a directory");
		LEEP_ASSERT(handle_ == ConstantValues::UNINITIALIZED_HANDLER, "This texture has been created before");
        Renderer &r = GM.renderer();

        if (!r.aviable_tex_pos_.empty())
        {
            handle_ = r.aviable_tex_pos_.front();
            r.aviable_tex_pos_.pop_front();
        }
        else
        {
            r.textures_.emplace_back();
            handle_ = (int32_t)r.textures_.size() - 1;
        }

        r.textures_[handle_].internal_id_ = 0;
        r.textures_[handle_].path_ = path;
        r.textures_[handle_].version_ = 0;
        r.textures_[handle_].width_ = 0;
        r.textures_[handle_].height_ = 0;
        r.textures_[handle_].cube_ = cube;
        r.textures_[handle_].linear_ = linear;
    }

    void Texture::createEmpty(float width, float height)
    {
        LEEP_ASSERT(handle_ == ConstantValues::UNINITIALIZED_HANDLER, "This texture has been created before");
		LEEP_ASSERT(width > 0.0f && height > 0.0f, "Width and height of the texture must be greater than 0");
        Renderer &r = GM.renderer();
        if (!r.aviable_tex_pos_.empty())
        {
            handle_ = r.aviable_tex_pos_.front();
            r.aviable_tex_pos_.pop_front();
        }
        else 
        {
            InternalTexture tmp;
            r.textures_.push_back(tmp);
            handle_ = (int32_t)r.textures_.size() - 1;
        }

        LEEP_CORE_ASSERT(((width > 1.0f && height > 1.0f) ||
            (width <= 1.0f && height <= 1.0f)),
            "Or absolute or relative, make a decision!");
        if (width > 1.0f)
        {
            r.textures_[handle_].width_ = (uint32_t)width;
            r.textures_[handle_].height_ = (uint32_t)height;
        }
        else
        {
            r.textures_[handle_].width_ =
            (uint32_t)((float)GM.window().width() * width);
            r.textures_[handle_].height_ =
            (uint32_t)((float)GM.window().height() * height);
        }

        r.textures_[handle_].internal_id_ = 0;
        r.textures_[handle_].path_ = "";
        r.textures_[handle_].version_ = 0;
		r.textures_[handle_].cube_ = false; // TODO: Create empty cube tex if I add pointlight shadows
		r.textures_[handle_].linear_ = false;
    }

    void Texture::release()
    {
        if (handle_ >= 0)
        {
            GM.renderer().textures_[handle_].version_ = -1;
			handle_ = ConstantValues::DELETED_HANDLER;
        }
    }

    int32_t Texture::handle() const
    {
        return handle_;
    }
}