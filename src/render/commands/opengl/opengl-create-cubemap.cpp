#include "render/commands/create-cubemap.h"
#include "core/manager.h"
#include "render/renderer.h"
#include <glad/glad.h>

namespace leep
{
    void CreateCubemap::executeCommand() const
    {
        Renderer &r = GM.renderer();
        int32_t id = texture_.handle();

        LEEP_CORE_ASSERT(r.textures_[id].version_ == 0,
            "Texture created before?");
        LEEP_CORE_ASSERT(id < 60,
            "Start thinking about the max textures");
        LEEP_CORE_ASSERT(id >= 0, "Texture not initialized");
        LEEP_CORE_ASSERT(r.textures_[id].internal_id_ == 0,
            "Renderer::createCubemap: Texture created before");
        LEEP_ASSERT(r.textures_[id].path_ != "",
            "The cubemap needs a path to a directory");
        LEEP_CORE_ASSERT(r.textures_[id].type_ == TexType::CUBE,
            "Incorrect texture type for cubemap");

        // The path for cubemaps will be the directory where the skyboxfaces are
        // and inside the directory the faces must have these names
        std::string faces[6] = {
            r.textures_[id].path_ + "/right.jpg",
            r.textures_[id].path_ + "/left.jpg",
            r.textures_[id].path_ + "/up.jpg",
            r.textures_[id].path_ + "/down.jpg",
            r.textures_[id].path_ + "/front.jpg",
            r.textures_[id].path_ + "/back.jpg",
        };

        glGenTextures(1, (GLuint*)&(r.textures_[id].internal_id_));
        r.textures_[id].texture_unit_ = id + 1;
        glActiveTexture(GL_TEXTURE0 + r.textures_[id].texture_unit_);
        glBindTexture(GL_TEXTURE_CUBE_MAP, r.textures_[id].internal_id_);
        
        int32_t width, height, nchannels;
        stbi_set_flip_vertically_on_load(0);
        for (uint32_t i = 0; i < 6; ++i)
        {

            uint8_t *img_data = stbi_load(
                faces[i].c_str(), &width, &height, &nchannels, 0);
            LEEP_CORE_ASSERT(img_data,"Couldn't load the image to the cubemap");
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB,
                width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
            stbi_image_free(img_data);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        r.textures_[id].version_++;
    }
}