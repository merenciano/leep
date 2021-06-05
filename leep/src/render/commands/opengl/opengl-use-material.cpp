#include "render/commands/use-material.h"
#include "core/manager.h"
#include "render/renderer.h"
#include <glad/glad.h>

#include <stdint.h>

namespace leep 
{
    void UseMaterial::executeCommand() const
    {
        //LEEP_CHECK_RESOURCE(irr_);
        Renderer &r = GM.renderer();
        GLuint program = r.materials_[type_]->internal_id();
        glUseProgram(program);
        uint32_t uniform_pos = glGetUniformLocation(program, "u_scene_data");
        glUniform4fv(uniform_pos, fcount_ / 4, fdata_);
        uniform_pos = glGetUniformLocation(program, "u_scene_tex");
        glUniform1iv(uniform_pos, cube_start_, idata_);
        uniform_pos = glGetUniformLocation(program, "u_scene_cube");
        glUniform1iv(uniform_pos, icount_ - cube_start_, idata_ + cube_start_);
    }
}