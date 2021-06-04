#include "render/commands/render-options.h"
#include "core/common-defs.h"

#include "glad/glad.h"

namespace leep
{
    void RenderOptions::executeCommand() const
    {
        // Blend options
        if (current_options_.dirty_mask_ & LEEP_BLEND_FUNC_BIT)
        {
            GLuint sfac, dfac;
            switch(current_options_.sfactor_)
            {
                case BlendFunc::ONE:
                    sfac = GL_ONE;
                    break;

                case BlendFunc::SRC_ALPHA:
                    sfac = GL_SRC_ALPHA;
                    break;

                case BlendFunc::ONE_MINUS_SRC_ALPHA:
                    sfac = GL_ONE_MINUS_SRC_ALPHA;
                    break;
                
                case BlendFunc::ZERO:
                    sfac = GL_ZERO;
                    break;

                default:
                    LEEP_CORE_ASSERT(false,
                        "RenderOption invalid BlendCommand S value");
                    break;
            }

            switch(current_options_.dfactor_)
            {
                case BlendFunc::ONE:
                    dfac = GL_ONE;
                    break;
                
                case BlendFunc::SRC_ALPHA:
                    dfac = GL_SRC_ALPHA;
                    break;

                case BlendFunc::ONE_MINUS_SRC_ALPHA:
                    dfac = GL_ONE_MINUS_SRC_ALPHA;
                    break;
                
                case BlendFunc::ZERO:
                    dfac = GL_ZERO;
                    break;

                default:
                    LEEP_CORE_ASSERT(false,
                        "RenderOption invalid BlendCommand D value");
                    break;
            }

            glBlendFunc(sfac, dfac);
        }

        if (current_options_.dirty_mask_ & LEEP_ENABLE_BLEND_BIT)
        {
            if (current_options_.enable_blend_)
            {
                glEnable(GL_BLEND);
            }
            else
            {
                glDisable(GL_BLEND);
            }
        }

        // Cull options
        if (current_options_.dirty_mask_ & LEEP_CULL_FACE_BIT)
        {
            switch(current_options_.cull_face_)
            {
                case CullFace::DISABLED:
                    glDisable(GL_CULL_FACE);
                    break;

                case CullFace::FRONT:
                    glEnable(GL_CULL_FACE);
                    glCullFace(GL_FRONT);
                    break;
                
                case CullFace::BACK:
                    glEnable(GL_CULL_FACE);
                    glCullFace(GL_BACK);
                    break;
                
                case CullFace::FRONT_AND_BACK:
                    glEnable(GL_CULL_FACE);
                    glCullFace(GL_FRONT_AND_BACK);
                    break;

                default:
                    LEEP_CORE_ASSERT(false,
                        "RenderOption invalid CullFace value");
                    break;
            }
        }

        // Depth options
        if (current_options_.dirty_mask_ & LEEP_WRITE_DEPTH_BIT)
        {
            // GL_TRUE is 1 and GL_FALSE 0 so this should work...
            glDepthMask(current_options_.enable_write_depth_);
        }

        if (current_options_.dirty_mask_ & LEEP_DEPTH_TEST_BIT)
        {
            if (current_options_.enable_depth_test_ == true)
            {
                glEnable(GL_DEPTH_TEST);
            }
            else
            {
                glDisable(GL_DEPTH_TEST);
            }
        }
    }
}