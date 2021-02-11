#include "render/commands/create-framebuffer.h"
#include "core/manager.h"
#include "render/renderer.h"
#include "render/commands/create-texture.h"
#include <glad/glad.h>

namespace leep
{
    void CreateFramebuffer::executeCommand() const
    {
        Renderer &r = GM.renderer();
        int32_t id = framebuffer_.id();
		InternalFramebuffer &ifb = r.framebuffers_[id];

        LEEP_CORE_ASSERT(ifb.version_ == 0, "Framebuffer created before?");
        LEEP_CORE_ASSERT(id >= 0, "Framebuffer not initialized");

        glGenFramebuffers(1, (GLuint*)&(ifb.internal_id_));
        glBindFramebuffer(GL_FRAMEBUFFER, ifb.internal_id_);
        
		if (ifb.color_)
		{
			CreateTexture()
				.set_texture(framebuffer_.color())
				.set_format(TextureFormat::COLOR_BUFFER)
				.executeCommand();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
								   r.textures_[framebuffer_.color().id()].internal_id_, 0);
		}
		if (ifb.depth_)
		{
			CreateTexture()
				.set_texture(framebuffer_.depth())
				.set_format(TextureFormat::DEPTH_BUFFER)
				.executeCommand();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
								   r.textures_[framebuffer_.depth().id()].internal_id_, 0);
			
		}

		r.framebuffers_[id].version_++;
    }
}