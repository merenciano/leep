#include "render/commands/equirectangular-to-cubemap.h"

#include "core/manager.h"
#include "render/renderer.h"
#include "render/internal-resources.h"
#include "render/commands/create-cubemap.h"
#include "render/commands/create-texture.h"
#include "render/commands/draw.h"
#include "render/commands/render-options.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace leep
{
    void EquirectangularToCubemap::executeCommand() const
    {
        Renderer &r = GM.renderer();
        InternalTexture &icu = r.textures_[out_cube_.handle()];

        if (icu.cpu_version_ > icu.gpu_version_)
        {
            CreateCubemap().set_texture(out_cube_).executeCommand();
        }

        glm::mat4 proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        glm::mat4 views[] =
        {
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        };

        GLuint fb;
        glGenFramebuffers(1, &fb);
        glBindFramebuffer(GL_FRAMEBUFFER, fb);
        glViewport(0, 0, icu.width_, icu.height_);
        // Manually sync framebuffer here after changing textures
        Material m;
        Texture equirec;
        equirec.create(path_, TexType::RGB_F16);
        CreateTexture().set_texture(equirec).executeCommand();
        m.set_albedo(equirec);
        m.set_type(MT_EQUIREC_TO_CUBE);
        RenderOptions().set_cull_face(CullFace::DISABLED).executeCommand();

        for (int32_t i = 0; i < 6; ++i)
        {
            m.set_model(proj * views[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, icu.internal_id_, 0);
            glClear(GL_COLOR_BUFFER_BIT);
            Draw().set_geometry(Renderer::s_cube).set_material(m).executeCommand();
        }
        glDeleteFramebuffers(1, &fb);

        equirec.release();
    }
}