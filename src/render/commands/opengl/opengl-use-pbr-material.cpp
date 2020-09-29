#include "render/commands/use-pbr-material.h"
#include "core/manager.h"
#include "glad/glad.h"

#include <stdint.h>

namespace motoret
{
    void UsePbrMaterial::executeCommand() const
    {
        GLuint program = Manager::instance().renderer().materials_[MaterialType::MT_PBR]->internal_id();
        glUseProgram(program);
        uint32_t uniform_pos = glGetUniformLocation(program, "u_scene_data");
        glUniform4fv(uniform_pos, sizeof(PbrSceneData) / sizeof(float) / 4, (float*)&data_); // QUESTION: "this" instead of "&data_" ??
    }
}