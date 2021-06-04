#include "render/commands/use-skybox-material.h"

#include "core/manager.h"
#include "render/camera.h"
#include "render/renderer.h"

#include <stdint.h>

#include <glad/glad.h>

namespace leep 
{
    void UseSkyboxMaterial::executeCommand() const
    {
        glm::mat4 static_vp = GM.camera().static_view_projection();
        GLuint program = GM.renderer().materials_[MaterialType::MT_SKYBOX]->internal_id();
        glUseProgram(program);
        uint32_t uniform_pos = glGetUniformLocation(program, "u_scene_data");
        glUniform4fv(uniform_pos, 4, (GLfloat*)&static_vp);
    }
}