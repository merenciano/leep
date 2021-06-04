#include "render/commands/use-pbr-material.h"
#include "core/manager.h"
#include "render/renderer.h"
#include <glad/glad.h>

#include <stdint.h>

namespace leep 
{
    void UsePbrMaterial::executeCommand() const
    {
        LEEP_CHECK_RESOURCE(irr_);
        Renderer &r = GM.renderer();
        GLuint program = r.materials_[MaterialType::MT_PBR]->internal_id();
        glUseProgram(program);
        uint32_t uniform_pos = glGetUniformLocation(program, "u_scene_data");
        glUniform4fv(uniform_pos, sizeof(PbrSceneData) / sizeof(float) / 4, (float*)&data_);

        uniform_pos = glGetUniformLocation(program, "u_irradiance_map");
        glUniform1i(uniform_pos, r.textures_[irr_.handle()].texture_unit_);
        uniform_pos = glGetUniformLocation(program, "u_prefilter_map");
        glUniform1i(uniform_pos, r.textures_[pref_.handle()].texture_unit_);
        uniform_pos = glGetUniformLocation(program, "u_lut");
        glUniform1i(uniform_pos, r.textures_[lut_.handle()].texture_unit_);
    }
}