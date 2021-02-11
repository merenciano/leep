#include "render/commands/create-texture.h"
#include "core/manager.h"
#include "render/renderer.h"
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
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
        Renderer &r = GM.renderer();
        int32_t id = texture_.id();
		TextureConfig config;

		switch(format_)
		{
			case TextureFormat::LINEAR: 
				config.format = GL_RGB;
				config.internal_format = GL_RGB;
				config.type = GL_UNSIGNED_BYTE;
				config.filter = GL_NEAREST; // TODO: Check with linear (dont want any pixeled img)
				config.wrap = GL_REPEAT;
				break;

			case TextureFormat::GAMMA: 
				config.format = GL_RGB;
				config.internal_format = GL_SRGB;
				config.type = GL_UNSIGNED_BYTE;
				config.filter = GL_NEAREST;
				config.wrap = GL_REPEAT;
				break;

			case TextureFormat::COLOR_BUFFER:
				config.format = GL_RGBA;
				config.internal_format = GL_RGBA16F;
				config.type = GL_FLOAT;
				config.filter = GL_LINEAR;
				config.filter = GL_CLAMP_TO_EDGE;
				break;

			case TextureFormat::DEPTH_BUFFER:
				config.format = GL_DEPTH_COMPONENT;
				config.internal_format = GL_DEPTH_COMPONENT;
				config.type = GL_FLOAT;
				config.filter = GL_LINEAR;
				config.wrap = GL_CLAMP_TO_BORDER;
				break;

			default: 
				LEEP_CORE_ERROR("Trying to create a texture with an invalid format");
				config.format = GL_INVALID_ENUM;
				config.internal_format = GL_INVALID_ENUM;
				config.type = GL_INVALID_ENUM;
				config.filter = GL_INVALID_ENUM;
				config.wrap = GL_INVALID_ENUM;
				break;
		}

        LEEP_CORE_ASSERT(r.textures_[id].version_ == 0, "Texture created before?");
        LEEP_CORE_ASSERT(id < 63, "Max texture units");
        LEEP_CORE_ASSERT(id >= 0, "Texture not initialized");
        LEEP_CORE_ASSERT(r.textures_[id].internal_id_ == 0, "Renderer::createTexture: Texture created before");

        glGenTextures(1, (GLuint*)&(r.textures_[id].internal_id_));
        r.textures_[id].texture_unit_ = id + 1;
        glActiveTexture(GL_TEXTURE0 + r.textures_[id].texture_unit_);
        glBindTexture(GL_TEXTURE_2D, r.textures_[id].internal_id_);
        
        if (r.textures_[id].path_ != "")
        {
            int32_t width, height, nchannels;
            stbi_set_flip_vertically_on_load(1);
            uint8_t *img_data = stbi_load(
                r.textures_[id].path_.c_str(),
                &width, &height, &nchannels, STBI_rgb);
            LEEP_CORE_ASSERT(img_data, "Can not load the image to the texture");
            glTexImage2D(GL_TEXTURE_2D, 0, config.internal_format, width, height,
                        0, config.format, config.type, img_data);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(img_data);
        }
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, config.internal_format, r.textures_[id].width_,
				r.textures_[id].height_, 0, config.format, config.type, NULL);
		}
		// No shadows outside shadow maps
		float border_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, config.filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, config.filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, config.wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, config.wrap);
        r.textures_[id].version_++;
    }
}