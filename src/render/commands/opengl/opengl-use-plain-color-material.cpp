#include "render/commands/use-plain-color-material.h"
#include "core/manager.h"
#include "glad/glad.h"

#include <stdint.h>

namespace motoret
{
    void UsePlainColorMaterial::executeCommand() const
    {
        GLuint program = Manager::instance().renderer().materials_[MaterialType::MT_PLAIN_COLOR]->internal_id();
        glUseProgram(program);
    }
}