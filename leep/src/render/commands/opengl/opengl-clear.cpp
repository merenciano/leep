#include "render/commands/clear.h"
#include "core/manager.h"
#include "render/camera.h"
#include "render/commands/use-framebuffer.h"
#include <stdint.h>

#include "glad/glad.h"

namespace leep
{
    void Clear::executeCommand() const
    {
        uint32_t mask = 0U;
        if (color_) mask |= GL_COLOR_BUFFER_BIT;
        if (depth_) mask |= GL_DEPTH_BUFFER_BIT;
        if (stencil_) mask |= GL_STENCIL_BUFFER_BIT;
        glClearColor(clear_color_[0], clear_color_[1],
                     clear_color_[2], clear_color_[3]);
        glClear(mask);
    }
}
