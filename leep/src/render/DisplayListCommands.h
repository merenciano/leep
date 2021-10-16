#ifndef LEEP_RENDER_DISPLAYLISTCOMMANDS_H
#define LEEP_RENDER_DISPLAYLISTCOMMANDS_H

#include "buffer.h"
#include "texture.h"
#include "framebuffer.h" // TODO: Think about the framebuffer API.. maybe only 1 and no user management?

namespace leep
{
    enum class CommandType
    {
        NONE,
        CLEAR,
        CREATE_BUFFER,
        CREATE_CUBEMAP,
        CREATE_FRAMEBUFFER,
        CREATE_TEXTURE,
        DRAW,
        DRAW_SKYBOX,
        EQUIRECTANGULAR_TO_CUBE,
        PREFILTER_MIPS,
        RENDER_OPTIONS,
        SWAP_BUFFERS,
        USE_FRAMEBUFFER,
        USE_MATERIAL,
    };

    enum CommandFlags : uint32_t
    {
        CLEAR_COLOR_FLAG                = 1 << 0,
        CLEAR_DEPTH_FLAG                = 1 << 1,
        CLEAR_STENCIL_FLAG              = 1 << 2,

        CREATE_TEXTURE_RELEASE_RAM_FLAG = 1 << 0,
    };

    union CommandData
    {
        struct ClearData
        {
            float clear_color[4];
            uint32_t flags;
        } clear_data;

        struct CreateBufferData
        {
            Buffer buffer;
        } create_buffer_data;

        struct CreateCubemapData
        {
            Texture cubemap;
        } create_cubemap_data;

        struct CreateTextureData
        {
            Texture tex;
            uint32_t flags;
        } create_texture_data;

        struct CreateFramebufferData
        {
            Framebuffer fb;
        };

        struct DrawData
        {

        };
    };

}

#endif //LEEP_RENDER_DISPLAYLISTCOMMANDS_H
