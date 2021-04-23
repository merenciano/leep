#include "render/commands/equirectangular-to-cubemap.h"

#include "core/manager.h"
#include "render/renderer.h"
#include "render/internal-resources.h"
#include "render/commands/create-cubemap.h"
#include "render/commands/create-texture.h"
#include "render/commands/draw.h"
#include "render/commands/render-options.h"

#include <cmath>
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
        equirec.create(path_.c_str(), TexType::RGB_F16);
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

        if (out_pref_.handle() != CommonDefs::UNINIT_HANDLE)
        {
            InternalTexture &ipref = r.textures_[out_pref_.handle()];
            if (ipref.cpu_version_ > ipref.gpu_version_)
            {
                CreateCubemap().set_texture(out_pref_).executeCommand();
            }
            Material m_pref;
            PrefilterEnvData pref_data;
            m_pref.set_type(MT_PREFILTER_ENV);
            m_pref.set_albedo(out_cube_);
            for (int32_t i = 0; i < 5; ++i)
            {
                // mip size
                uint32_t s = (uint32_t)((float)ipref.width_ * powf(0.5f, (float)i));
                glViewport(0, 0, s, s);
                pref_data.roughness_ = (float)i / 4.0f;  // mip / max mip levels - 1

                for (int32_t j = 0; j < 6; ++j)
                {
                    pref_data.vp_ = proj * views[j];
                    m_pref.set_data(pref_data);
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                        GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, ipref.internal_id_, i);
                    glClear(GL_COLOR_BUFFER_BIT);
                    Draw().set_geometry(Renderer::s_cube).set_material(m_pref).executeCommand();
                }
            }
        }

        if (out_lut_.handle() != CommonDefs::UNINIT_HANDLE)
        {
            InternalTexture &ilut = r.textures_[out_lut_.handle()];
            Material mlut;
            mlut.set_type(MT_LUT_GEN);
            if (ilut.cpu_version_ > ilut.gpu_version_)
            {
                CreateTexture().set_texture(out_lut_).executeCommand();
            }
            glViewport(0, 0, ilut.width_, ilut.height_);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ilut.internal_id_, 0);
            glClear(GL_COLOR_BUFFER_BIT);
            Draw().set_geometry(Renderer::s_quad).set_material(mlut).executeCommand();
        }

        glDeleteFramebuffers(1, &fb);
        equirec.release();
    }
}