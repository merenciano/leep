#include "render/use-pbr-material.h"
#include "core/manager.h"
#include "glad/glad.h"

#include <stdint.h>

namespace motoret
{
    void UsePbrMaterial::executeCommand() const
    {
        GLuint program = Manager::instance().renderer().materials_[MaterialType::PBR].program;
        glUseProgram(program);
        uint32_t uniform_pos = glGetUniformLocation(program, "u_scene_data");
        glUniform4fv(uniform_pos, sizeof(PbrSceneData) / sizeof(float) / 4, &data_); // QUESTION: "this" instead of "&data_" ??
    }
}