#ifndef LEEP_RENDERER_H
#define LEEP_RENDERER_H

namespace leep
{
    namespace display
    {
        enum class ECommands
        {
            NONE = 0,
            CLEAR,
            CREATE_BUFFER,
            CREATE_CUBEMAP,
            CREATE_FRAMEBUFFER,
            CREATE_TEXTURE,
            DRAW,
            DRAW_SKYBOX,
            EQUIRECTANGULAR_TO_CUBEMAP,
            PREFILTER_MIP_MAP,
            RENDER_OPTIONS,
            SWAP_BUFFERS,
            USE_FRAMEBUFFER,
            USE_MATERIAL
        };

        enum class EClear
        {
            COLOR = 1 << 0,
            DEPTH = 1 << 1,
            STENCIL 1 << 2
        };

        struct SClearData
        {
            EClear Flags;
            float Red;
            float Green;
            float Blue;
        };

        struct SCommand
        {
            ECommands mType;
            union
            {
                SClearData ClearData;
            };
        };
    }
}

typedef struct
{
    command_t command;
    union
} leep_display_command_t;

#endif //LEEP_RENDERER_H
