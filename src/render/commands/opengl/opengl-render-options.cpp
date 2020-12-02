#include "render/commands/render-options.h"
#include "core/common-defs.h"

#include "glad/glad.h"

namespace leep
{
    void RenderOptions::executeCommand() const
    {
        // Blend options
        GLuint sfac, dfac;
        switch(s_current_options.sfactor)
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
                LEEP_CORE_ASSERT(false, "RenderOption invalid BlendCommand S value");
                break;
        }

        switch(s_current_options.dfactor)
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
                LEEP_CORE_ASSERT(false, "RenderOption invalid BlendCommand D value");
                break;
        }

        glBlendFunc(sfac, dfac);

        // Cull options
        GLuint cull;
        switch(s_current_options.cull_face)
        {
            case CullFace::FRONT:
                cull = GL_FRONT;
                break;
            
            case CullFace::BACK:
                cull = GL_BACK;
                break;
            
            case CullFace::FRONT_AND_BACK:
                cull = GL_FRONT_AND_BACK;
                break;
            
            default:
                LEEP_CORE_ASSERT(false, "RenderOption invalid CullFace value");
                break;
        }

        glCullFace(cull);

        // Depth options
        if (s_current_options.write == true) 
        {
            glDepthMask(GL_TRUE);
        }
        else
        {
            glDepthMask(GL_FALSE);
        }

        if (s_current_options.read == true)
        {
            glEnable(GL_DEPTH_TEST);
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }
    }
}