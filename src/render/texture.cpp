#include "texture.h"

#include "core/common-defs.h"
#include "core/manager.h"
#include "core/window.h"
#include "render/renderer.h"

namespace leep
{
    Texture::Texture()
    {
        handle_ = CommonDefs::UNINIT_HANDLE;
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

    void Texture::create(const char *path, TexType t)
    {
        LEEP_ASSERT(handle_ == CommonDefs::UNINIT_HANDLE,
            "This texture is currently in use");
        LEEP_ASSERT(*path != '\0', "For empty textures use createEmpty");

        Renderer &r = GM.renderer();

        if (!r.aviable_tex_pos_.empty())
        {
            handle_ = r.aviable_tex_pos_.front();
            r.aviable_tex_pos_.pop_front();
        }
        else
        {
            handle_ = r.addTex();
        }

        r.textures_[handle_].internal_id_ = CommonDefs::UNINIT_INTERNAL_ID;
        strcpy(r.textures_[handle_].path_, path);
        r.textures_[handle_].cpu_version_ = 1;
        r.textures_[handle_].gpu_version_ = 0;
        r.textures_[handle_].width_ = 0;
        r.textures_[handle_].height_ = 0;
        r.textures_[handle_].type_ = t;
    }

    void Texture::createEmpty(float width, float height, TexType t)
    {
        LEEP_ASSERT(handle_ == CommonDefs::UNINIT_HANDLE, "This texture is currently in use");
		LEEP_ASSERT(width > 0.0f && height > 0.0f,
            "Width and height of the texture must be greater than 0");
        Renderer &r = GM.renderer();
        if (!r.aviable_tex_pos_.empty())
        {
            handle_ = r.aviable_tex_pos_.front();
            r.aviable_tex_pos_.pop_front();
        }
        else 
        {
            handle_ = r.addTex();
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
            r.textures_[handle_].width_ = (uint32_t)(GM.window().fwidth() * width);
            r.textures_[handle_].height_ = (uint32_t)(GM.window().fheight() * height);
        }

        r.textures_[handle_].internal_id_ = CommonDefs::UNINIT_INTERNAL_ID;
        *(r.textures_[handle_].path_) = '\0';
        r.textures_[handle_].cpu_version_ = 1;
        r.textures_[handle_].gpu_version_ = 0;
		r.textures_[handle_].type_ = t;
    }

    void Texture::release()
    {
        if (handle_ >= 0)
        {
            GM.renderer().textures_[handle_].cpu_version_ = CommonDefs::DELETED_GPU_RESOURCE;
            GM.renderer().textures_[handle_].gpu_version_ = CommonDefs::DELETED_GPU_RESOURCE;

			handle_ = CommonDefs::DELETED_HANDLE;
        }
    }

    int32_t Texture::handle() const
    {
        return handle_;
    }
} // namespace leep
