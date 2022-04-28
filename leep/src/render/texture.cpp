#include "texture.h"

#include "core/common-defs.h"
#include "core/manager.h"
#include "core/window.h"
#include "render/renderer.h"

namespace leep
{
    Texture::Texture()
    {
        //handle_ = CommonDefs::UNINIT_HANDLE;
	    handle_ = THE_UNINIT;
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

    THE_TexType ToCType(TexType t)
    {
	    switch (t)
	    {
	    case TexType::DEPTH: return THE_TEX_DEPTH;
	    case TexType::ENVIRONMENT: return THE_TEX_ENVIRONMENT;
	    case TexType::LUT: return THE_TEX_LUT;
	    case TexType::NONE: return THE_TEX_NONE;
	    case TexType::PREFILTER_ENVIRONMENT: return THE_TEX_PREFILTER_ENVIRONMENT;
	    case TexType::R: return THE_TEX_R;
	    case TexType::RGB: return THE_TEX_RGB;
	    case TexType::RGB_F16: return THE_TEX_RGB_F16;
	    case TexType::SRGB: return THE_TEX_SRGB;
	    case TexType::RGBA_F16: return THE_TEX_RGBA_F16;
	    case TexType::SKYBOX: return THE_TEX_SKYBOX;
	    }
    }

    void Texture::create(const char *path, TexType t)
    {
	    handle_ = THE_CreateTexture(path, ToCType(t));
	    /*
        LEEP_ASSERT(handle_ == CommonDefs::UNINIT_HANDLE,
            "This texture is currently in use");
        LEEP_ASSERT(*path != '\0', "For empty textures use createEmpty");

        Renderer &r = GM.renderer();
        r.mtx_.lock();
        if (!r.aviable_tex_pos_.empty())
        {
            handle_ = r.aviable_tex_pos_.front();
            r.aviable_tex_pos_.pop_front();
        }
        else
        {
            handle_ = r.addTex();
        }
        r.mtx_.unlock();
        r.textures_[handle_].internal_id_ = CommonDefs::UNINIT_INTERNAL_ID;
        strcpy(r.textures_[handle_].path_, path);
        r.textures_[handle_].pix_ = nullptr;
        r.textures_[handle_].cpu_version_ = 1;
        r.textures_[handle_].gpu_version_ = 0;
        r.textures_[handle_].width_ = 0;
        r.textures_[handle_].height_ = 0;
        r.textures_[handle_].type_ = t*/
    }

    void Texture::createEmpty(float width, float height, TexType t)
    {
	    if (width > 1.0f)
	    {
		    handle_ = THE_CreateEmptyTexture((u32)width, (u32)height, ToCType(t));
	    }
	    else
	    {
		    handle_ = THE_CreateEmptyTexture(width, height, ToCType(t));
	    }
	   
        /*LEEP_ASSERT(handle_ == CommonDefs::UNINIT_HANDLE, "This texture is currently in use");
		LEEP_ASSERT(width > 0.0f && height > 0.0f,
            "Width and height of the texture must be greater than 0");
        Renderer &r = GM.renderer();
        r.mtx_.lock();
        if (!r.aviable_tex_pos_.empty())
        {
            handle_ = r.aviable_tex_pos_.front();
            r.aviable_tex_pos_.pop_front();
        }
        else 
        {
            handle_ = r.addTex();
        }
        r.mtx_.unlock();
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
        r.textures_[handle_].pix_ = nullptr;
        r.textures_[handle_].cpu_version_ = 1;
        r.textures_[handle_].gpu_version_ = 0;
		r.textures_[handle_].type_ = t;*/
    }

    void Texture::createAndLoad(const char *path, TexType t)
    {
	    handle_ = THE_CreateTexture(path, ToCType(t));
	    THE_LoadTexture(handle_, path);
	    /*
        LEEP_ASSERT(handle_ == CommonDefs::UNINIT_HANDLE,
            "This texture is currently in use");
        LEEP_ASSERT(*path != '\0', "For empty textures use createEmpty");

        Renderer &r = GM.renderer();

        // Lock needed because this function is called
        // from ResourceMap::addTextureAsync
        r.mtx_.lock();
        if (!r.aviable_tex_pos_.empty())
        {
            handle_ = r.aviable_tex_pos_.front();
            r.aviable_tex_pos_.pop_front();
        }
        else
        {
            handle_ = r.addTex();
        }
        r.mtx_.unlock();

        int width, height, nchannels = 0;
        stbi_set_flip_vertically_on_load(1);
        if (t == TexType::RGB_F16 || t == TexType::RGBA_F16 || t ==TexType::LUT)
        {
            r.textures_[handle_].pix_ = (void*)stbi_loadf(
                path, &width, &height, &nchannels, 0);
            LEEP_CORE_ASSERT(r.textures_[handle_].pix_,
                "The image couldn't be loaded");
        }
        else
        {
			int a = 0;
            if (t == TexType::RGB || t == TexType::SRGB)
            {
				a = 3;
                nchannels = 3;
            }
            else if (t == TexType::R)
            {
				a = 1;
                nchannels = 1;
            }
            r.textures_[handle_].pix_ = (void*)stbi_load(
                path, &width, &height, &nchannels, a);
            LEEP_CORE_ASSERT(r.textures_[handle_].pix_,"The image couldn't be loaded.");
        }

        r.textures_[handle_].internal_id_ = CommonDefs::UNINIT_INTERNAL_ID;
        strcpy(r.textures_[handle_].path_, path);
        r.textures_[handle_].cpu_version_ = 1;
        r.textures_[handle_].gpu_version_ = 0;
        r.textures_[handle_].width_ = width;
        r.textures_[handle_].height_ = height;
        r.textures_[handle_].type_ = t;

        if (path)
        {
            GM.memory().generalFree((void*)path);
        }
	*/
    }

    void Texture::releasePixels()
    {
	    THE_FreeTextureData(handle_);
	    /*
        if (GM.renderer().textures_[handle_].pix_)
        {
            stbi_image_free(GM.renderer().textures_[handle_].pix_);
            //GM.memory().generalFree(GM.renderer().textures_[handle_].pix_);
            GM.renderer().textures_[handle_].pix_ = nullptr;
        }
	*/
    }

    void Texture::release()
    {
	    THE_ReleaseTexture(handle_);
        /*if (handle_ >= 0)
        {
            GM.renderer().textures_[handle_].cpu_version_ = CommonDefs::DELETED_GPU_RESOURCE;
            GM.renderer().textures_[handle_].gpu_version_ = CommonDefs::DELETED_GPU_RESOURCE;
            releasePixels();

			handle_ = CommonDefs::DELETED_HANDLE;
        }*/
    }

    int32_t Texture::handle() const
    {
        return handle_;
    }
} // namespace leep
