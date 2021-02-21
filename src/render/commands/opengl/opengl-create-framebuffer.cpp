#include "render/commands/create-framebuffer.h"
#include "core/manager.h"
#include "render/renderer.h"
#include "render/commands/create-texture.h"
#include <glad/glad.h>

namespace leep {
/* Trying to substitute create commands with update or sync commands
void CreateFramebuffer::executeCommand() const
{
    Renderer &r = GM.renderer();
    int32_t id = fb_.handle();
    InternalFramebuffer &ifb = r.framebuffers_[id];

    LEEP_CORE_ASSERT(ifb.gpu_version_ == 0, "Internal framebuffer created before");
    LEEP_CORE_ASSERT(ifb.cpu_version_ > 0, "Framebuffer not created");

    glGenFramebuffers(1, (GLuint*)&(ifb.internal_id_));
    glBindFramebuffer(GL_FRAMEBUFFER, ifb.internal_id_);
    
    if (ifb.color_)
    {
        CreateTexture().set_texture(fb_.color()).executeCommand();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
            r.textures_[fb_.color().handle()].internal_id_, 0);
    }
    if (ifb.depth_)
    {
        CreateTexture().set_texture(fb_.depth()).executeCommand();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
            r.textures_[fb_.depth().handle()].internal_id_, 0);
        
    }

    ifb.gpu_version_ = ifb.cpu_version_;
}
*/

void CreateFramebuffer::executeCommand() const
{
    Renderer &r = GM.renderer();
    int32_t id = fb_.handle();
    InternalFramebuffer &ifb = r.framebuffers_[id];

    LEEP_CORE_ASSERT(!((ifb.color_ && ifb.depth_) && (
        (r.textures_[ifb.color_texture_.handle()].width_ != 
         r.textures_[ifb.depth_texture_.handle()].width_) || 
        (r.textures_[ifb.color_texture_.handle()].height_ !=
         r.textures_[ifb.depth_texture_.handle()].height_)
        )), "The size of the color and depth buffer has to be the same");

    if (ifb.internal_id_ == CommonDefs::UNINIT_INTERNAL_ID)
    {
        glGenFramebuffers(1, (GLuint*)&(ifb.internal_id_));
    }
    glBindFramebuffer(GL_FRAMEBUFFER, ifb.internal_id_);
    
    if (ifb.color_)
    {
        if (r.textures_[fb_.color().handle()].gpu_version_ == 0)
        {
            CreateTexture().set_texture(fb_.color()).executeCommand();
        }
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D, r.textures_[fb_.color().handle()].internal_id_, 0);
        ifb.width_ = (float)r.textures_[fb_.color().handle()].width_;
        ifb.height_ = (float)r.textures_[fb_.color().handle()].height_;
    }
    if (ifb.depth_)
    {
        if (r.textures_[fb_.depth().handle()].gpu_version_ == 0)
        {
            CreateTexture().set_texture(fb_.depth()).executeCommand();
        }
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
            GL_TEXTURE_2D, r.textures_[fb_.depth().handle()].internal_id_, 0);
        ifb.width_ = (float)r.textures_[fb_.depth().handle()].width_;
        ifb.height_ = (float)r.textures_[fb_.depth().handle()].height_;
    }
    ifb.gpu_version_ = ifb.cpu_version_;
}

} // namespace leep
