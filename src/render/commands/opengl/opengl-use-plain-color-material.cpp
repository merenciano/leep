#include "render/commands/use-plain-color-material.h"
#include "core/manager.h"
#include "glad/glad.h"

#include <stdint.h>

namespace leep
{
    UsePlainColorMaterial& UsePlainColorMaterial::set_scene_data(const PlainColorSceneData &scene_data)
    {
        data_ = scene_data;
        return *this;
    }

    void UsePlainColorMaterial::executeCommand() const
    {
        GLuint program = Manager::instance().renderer().materials_[MaterialType::MT_PLAIN_COLOR]->internal_id();
        glUseProgram(program);
        uint32_t uniform_pos = glGetUniformLocation(program, "u_scene_data");
        glUniform4fv(uniform_pos, sizeof(PlainColorSceneData) / sizeof(float) / 4, (float*)&data_); // QUESTION: "this" instead of "&data_" ??
    }
}