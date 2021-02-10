#include "render/commands/create-framebuffer.h"
#include "core/manager.h"
#include "render/renderer.h"
#include "render/commands/create-texture.h"
#include <glad/glad.h>

namespace leep
{
    void CreateFramebuffer::executeCommand() const
    {
        //Renderer &r = GM.renderer();
        int32_t id = framebuffer_.id();
		InternalFramebuffer &ifb = GM.renderer().framebuffers_[id];

        LEEP_CORE_ASSERT(ifb.version_ == 0, "Framebuffer created before?");
        LEEP_CORE_ASSERT(id >= 0, "Framebuffer not initialized");

        glGenFramebuffers(1, (GLuint*)&(ifb.internal_id_));
        glBindFramebuffer(GL_FRAMEBUFFER, ifb.internal_id_);
        
		if (ifb.color_)
		{
			CreateTexture().set_texture(framebuffer_.color()).executeCommand();
			VOY POR AQUI
		}
    }
}