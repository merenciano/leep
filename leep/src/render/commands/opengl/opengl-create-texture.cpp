#include "render/commands/create-texture.h"
#include "core/manager.h"
#include "render/renderer.h"
#include <glad/glad.h>

#include "stb_image.h"

namespace leep
{
	struct TextureConfig
	{
		GLenum internal_format;
		GLenum format;
		GLenum type;
		GLenum wrap;
		GLenum filter;
	};

    void CreateTexture::executeCommand() const
    {
		TextureConfig config;
        int32_t id = texture_.handle();
        InternalTexture &itex = GM.renderer().textures_[id];

        LEEP_CORE_ASSERT(itex.cpu_version_ == 1, "Texture created before?");
        LEEP_CORE_ASSERT(id < 62, "Max texture units"); // Tex unit is id + 1
        LEEP_CORE_ASSERT(id >= 0, "Texture not initialized");
        LEEP_CORE_ASSERT(itex.internal_id_ == CommonDefs::UNINIT_INTERNAL_ID,
            "Texture already created on GPU");

		switch(itex.type_)
		{
			case TexType::R:
				config.format = GL_RED;
				config.internal_format = GL_R8;
				config.type = GL_UNSIGNED_BYTE;
				config.filter = GL_LINEAR;
				config.wrap = GL_REPEAT;
				break;

			case TexType::LUT:
				config.format = GL_RG;
				config.internal_format = GL_RG16F;
				config.type = GL_FLOAT;
				config.filter = GL_LINEAR;
				config.wrap = GL_CLAMP_TO_EDGE;

				break;

			case TexType::RGB: 
				config.format = GL_RGB;
				config.internal_format = GL_RGB;
				config.type = GL_UNSIGNED_BYTE;
				config.filter = GL_LINEAR;
				config.wrap = GL_REPEAT;
				break;

			case TexType::SRGB: 
				config.format = GL_RGB;
				config.internal_format = GL_SRGB;
				config.type = GL_UNSIGNED_BYTE;
				config.filter = GL_LINEAR;
				config.wrap = GL_REPEAT;
				break;

			case TexType::RGBA_F16:
				config.format = GL_RGBA;
				config.internal_format = GL_RGBA16F;
				config.type = GL_FLOAT;
				config.filter = GL_LINEAR;
				config.wrap = GL_CLAMP_TO_EDGE;
				break;

			case TexType::DEPTH:
				config.format = GL_DEPTH_COMPONENT;
				config.internal_format = GL_DEPTH_COMPONENT;
				config.type = GL_FLOAT;
				config.filter = GL_LINEAR;
				config.wrap = GL_CLAMP_TO_BORDER;
				break;

            case TexType::RGB_F16:
                config.format = GL_RGB;
                config.internal_format = GL_RGB16F;
                config.type = GL_FLOAT;
                config.filter = GL_LINEAR;
                config.wrap = GL_CLAMP_TO_EDGE;
                break;

			default: 
				LEEP_CORE_ERROR("Invalid format");
				config.format = GL_INVALID_ENUM;
				config.internal_format = GL_INVALID_ENUM;
				config.type = GL_INVALID_ENUM;
				config.filter = GL_INVALID_ENUM;
				config.wrap = GL_INVALID_ENUM;
				break;
		}


        glGenTextures(1, (GLuint*)&(itex.internal_id_));
        itex.texture_unit_ = id + 1;
        glActiveTexture(GL_TEXTURE0 + itex.texture_unit_);
        glBindTexture(GL_TEXTURE_2D, itex.internal_id_);
        
        if (itex.type_ == TexType::RGB_F16)
        {
            if (!itex.pix_)
            {
				stbi_set_flip_vertically_on_load(1);
				int width, height, nchannels;
                itex.pix_ = stbi_loadf(
                    itex.path_, &width, &height, &nchannels, 0);
				itex.width_ = width;
				itex.height_ = height;
            }
            LEEP_CORE_ASSERT(itex.pix_, "The image couldn't be loaded");
            glTexImage2D(GL_TEXTURE_2D, 0, config.internal_format, itex.width_,
				itex.height_, 0, config.format, config.type, itex.pix_);
            if (release_ram_data_)
            {
                stbi_image_free(itex.pix_);
                itex.pix_ = nullptr;
            }
        }
        else if (itex.path_[0] != '\0')
        {
            if (!itex.pix_)
            {
				stbi_set_flip_vertically_on_load(1);
				int width, height, nchannels;
                itex.pix_ = stbi_load(
                    itex.path_, &width, &height, &nchannels, STBI_rgb);
				itex.width_ = width;
				itex.height_ = height;
            }
            LEEP_CORE_ASSERT(itex.pix_, "The image couldn't be loaded");
            glTexImage2D(GL_TEXTURE_2D, 0, config.internal_format, itex.width_,
				itex.height_, 0, config.format, config.type, itex.pix_);
            glGenerateMipmap(GL_TEXTURE_2D);
            if (release_ram_data_)
            {
                stbi_image_free(itex.pix_);
                itex.pix_ = nullptr;
            }
        }
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, config.internal_format, itex.width_,
                itex.height_, 0, config.format, config.type, NULL);
		}
		// No shadows outside shadow maps
		float border_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, config.filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, config.filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, config.wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, config.wrap);
        itex.gpu_version_ = itex.cpu_version_;
    }
}
