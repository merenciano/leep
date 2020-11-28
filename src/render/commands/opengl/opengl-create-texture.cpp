#include "render/commands/create-texture.h"
#include "glad/glad.h"
#include "core/manager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace leep
{
    void CreateTexture::executeCommand() const
    {
        int32_t id = texture_.id();
        LEEP_CORE_ASSERT(textures_[id].version_ == 0, "Texture created before?");
        LEEP_CORE_ASSERT(id < 60, "Start thinking about the max textures");
        LEEP_CORE_ASSERT(id >= 0, "Texture not initialized");
        LEEP_CORE_ASSERT(textures_[id].id_ == 0, "Renderer::createTexture: Texture created before");
        glGenTextures(1, (GLuint*)&(GM.renderer().textures_[id].id_));
        GM.renderer().textures_[id].texture_unit_ = id + 1;
        glActiveTexture(GL_TEXTURE0 + GM.renderer().textures_[id].texture_unit_);
        glBindTexture(GL_TEXTURE_2D, GM.renderer().textures_[id].id_);
        
        if (GM.renderer().textures_[id].path_ != "")
        {
            int32_t width, height, nchannels;
            stbi_set_flip_vertically_on_load(1);
            uint8_t *img_data = stbi_load(
                GM.renderer().textures_[id].path_.c_str(),
                &width, &height, &nchannels, STBI_rgb);
            LEEP_CORE_ASSERT(img_data, "Can not load the image to the texture");
            glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height,
                        0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(img_data);
        }
        else if (GM.renderer().textures_[id].width_ > 0)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GM.renderer().textures_[id].width_,
            GM.renderer().textures_[id].height_, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        GM.renderer().textures_[id].version_++;
    }
}