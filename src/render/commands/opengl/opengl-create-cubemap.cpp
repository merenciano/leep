#include "render/commands/create-cubemap.h"
#include "core/manager.h"
#include "render/renderer.h"
#include <glad/glad.h>

namespace leep
{
    struct CubeConfig
    {
        GLenum internal_format;
        GLenum format;
        GLenum type;
        GLenum wrap;
        GLenum filter;
    };

    void CreateCubemap::executeCommand() const
    {
        CubeConfig config;
        Renderer &r = GM.renderer();
        int32_t id = texture_.handle();

        LEEP_CORE_ASSERT(r.textures_[id].version_ == 0,
            "Texture created before (CPU)");
        LEEP_CORE_ASSERT(id < 60,
            "Start thinking about the max textures");
        LEEP_CORE_ASSERT(id >= 0, "Texture not initialized");
        LEEP_CORE_ASSERT(r.textures_[id].internal_id_ == 0,
            "Renderer::createCubemap: Texture created before (GPU)"); // TODO: textures with cpu and gpu version

        switch (r.textures_[id].type_)
        {
        case TexType::SKYBOX:
            config.format = GL_RGB;
            config.internal_format = GL_SRGB;
            config.type = GL_UNSIGNED_BYTE;
            config.filter = GL_LINEAR;
            config.wrap = GL_CLAMP_TO_EDGE;
            break;

        case TexType::ENVIRONMENT:
            config.format = GL_RGB;
            config.internal_format = GL_RGB16F;
            config.type = GL_FLOAT;
            config.filter = GL_LINEAR;
            config.wrap = GL_CLAMP_TO_EDGE;
            break;

        default:
            LEEP_CORE_ERROR("Trying to create a cubemap with an invalid format");
            config.format = GL_INVALID_ENUM;
            config.internal_format = GL_INVALID_ENUM;
            config.type = GL_INVALID_ENUM;
            config.filter = GL_INVALID_ENUM;
            config.wrap = GL_INVALID_ENUM;
            break;
        }

        glGenTextures(1, (GLuint*)&(r.textures_[id].internal_id_));
        r.textures_[id].texture_unit_ = id + 1;
        glActiveTexture(GL_TEXTURE0 + r.textures_[id].texture_unit_);
        glBindTexture(GL_TEXTURE_CUBE_MAP, r.textures_[id].internal_id_);

        // The path for cubemaps will be the directory where the skyboxfaces are
        // and inside the directory the faces must have these names
        if (r.textures_[id].path_ != "")
        {
            std::string faces[6] = {
                r.textures_[id].path_ + "/right.jpg",
                r.textures_[id].path_ + "/left.jpg",
                r.textures_[id].path_ + "/up.jpg",
                r.textures_[id].path_ + "/down.jpg",
                r.textures_[id].path_ + "/front.jpg",
                r.textures_[id].path_ + "/back.jpg",

            };

            int width, height, nchannels;
            stbi_set_flip_vertically_on_load(0);
            for (uint32_t i = 0; i < 6; ++i)
            {
                uint8_t* img_data = stbi_load(
                    faces[i].c_str(), &width, &height, &nchannels, 0);
                LEEP_CORE_ASSERT(img_data,"Couldn't load the image to the cubemap");
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, config.internal_format,
                    width, height, 0, config.format, config.type, img_data);
                stbi_image_free(img_data);
            }
            r.textures_[id].width_ = width;
            r.textures_[id].height_ = height;
        }
        else
        {
            LEEP_CORE_ASSERT(r.textures_[id].width_ > 0 && r.textures_[id].height_ > 0,
                "The texture have to have size for the empty environment");
            for (uint32_t i = 0; i < 6; ++i)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, config.internal_format,
                    r.textures_[id].width_, r.textures_[id].height_, 0, config.format, config.type, 0);
            }
        }
        
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, config.wrap);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, config.wrap);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, config.wrap);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, config.filter);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, config.filter);
        r.textures_[id].version_++;
    }
}