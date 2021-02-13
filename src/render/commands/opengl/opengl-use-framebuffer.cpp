#include "render/commands/use-framebuffer.h"
#include "render/commands/create-texture.h"
#include "core/manager.h"
#include "render/renderer.h"

#include <stdint.h>
#include <glad/glad.h>

namespace leep 
{
    void UseFramebuffer::executeCommand() const
    {
        if (default_)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            return;
        }

        Renderer &r = GM.renderer();
        InternalFramebuffer &ifb = r.framebuffers_[fb_.id()];
        GLsizei width;
        GLsizei height;

        if (ifb.gpu_version_ == 0)
        {
            LEEP_CORE_ASSERT(fb_.id() >= 0 && ifb.cpu_version_ > 0, "Framebuffer not created");

            glGenFramebuffers(1, (GLuint*)&(ifb.internal_id_));
            
            if (ifb.color_)
            {
                CreateTexture()
                    .set_texture(fb_.color())
                    .set_format(TextureFormat::COLOR_BUFFER)
                    .executeCommand();
            }
            if (ifb.depth_)
            {
                CreateTexture()
                    .set_texture(fb_.depth())
                    .set_format(TextureFormat::DEPTH_BUFFER)
                    .executeCommand();
            }
        }

        glBindFramebuffer(GL_FRAMEBUFFER, ifb.internal_id_);

        // Set viewport
        if (ifb.color_)
        {
            width = r.textures_[fb_.color().id()].width_;
            height = r.textures_[fb_.color().id()].height_;
            glViewport(0, 0, width, height);
        }

        if (ifb.depth_)
        {
            if (ifb.color_)
            {
                LEEP_CORE_ASSERT(width == r.textures_[fb_.depth().id()].width_ &&
                                 height == r.textures_[fb_.depth().id()].height_,
                                 "Color and depth texture sizes of framebuffer not matching");
            }
            else
            {
                width = r.textures_[fb_.depth().id()].width_;
                height = r.textures_[fb_.depth().id()].height_;
                glViewport(0, 0, width, height);
            }
        }

        // Update framebuffer if the textures have been changed
        if (ifb.gpu_version_ < ifb.cpu_version_)
        {
            if (ifb.color_)
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                    GL_TEXTURE_2D, r.textures_[fb_.color().id()].internal_id_, 0); 
            }

            if (ifb.depth_)
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                    GL_TEXTURE_2D, r.textures_[fb_.depth().id()].internal_id_, 0);
            }

            ifb.gpu_version_ = ifb.cpu_version_;
        }
    }
}