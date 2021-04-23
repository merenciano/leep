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
        InternalFramebuffer &ifb = r.framebuffers_[fb_.handle()];
        GLsizei width;
        GLsizei height;

        if (ifb.gpu_version_ == 0)
        {
            LEEP_CORE_ASSERT(fb_.handle() >= 0 && ifb.cpu_version_ > 0, "Framebuffer not created");

            glGenFramebuffers(1, (GLuint*)&(ifb.internal_id_));
            
            if (ifb.color_)
            {
                CreateTexture()
                    .set_texture(fb_.color())
                    .executeCommand();
            }
            if (ifb.depth_)
            {
                CreateTexture()
                    .set_texture(fb_.depth())
                    .executeCommand();
            }
        }

        glBindFramebuffer(GL_FRAMEBUFFER, ifb.internal_id_);

        // Set viewport
        if (ifb.color_)
        {
            width = r.textures_[fb_.color().handle()].width_;
            height = r.textures_[fb_.color().handle()].height_;
            glViewport(0, 0, width, height);
        }

        if (ifb.depth_)
        {
            if (ifb.color_)
            {
                LEEP_CORE_ASSERT(width == (GLsizei)r.textures_[fb_.depth().handle()].width_ &&
                                 height == (GLsizei)r.textures_[fb_.depth().handle()].height_,
                                 "Color and depth texture sizes of framebuffer not matching");
            }
            else
            {
                width = r.textures_[fb_.depth().handle()].width_;
                height = r.textures_[fb_.depth().handle()].height_;
                glViewport(0, 0, width, height);
            }
        }

        // Update framebuffer if the textures have been changed
        if (ifb.gpu_version_ < ifb.cpu_version_)
        {
            if (ifb.color_)
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                    GL_TEXTURE_2D, r.textures_[fb_.color().handle()].internal_id_, 0); 
            }

            if (ifb.depth_)
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                    GL_TEXTURE_2D, r.textures_[fb_.depth().handle()].internal_id_, 0);
            }

            ifb.gpu_version_ = ifb.cpu_version_;
        }
    }
}