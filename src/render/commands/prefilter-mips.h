// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_COMMANDS_PREFILTER_MIPS_H__
#define __LEEP_RENDER_COMMANDS_PREFILTER_MIPS_H__

#include "core/string.h"
#include "render/display-list-command.h"
#include "render/texture.h"

namespace leep
{
    class PrefilterMips : public DLComm
    {
    public:
        PrefilterMips();
        ~PrefilterMips();

        PrefilterMips &set_in_path(String in_path);
        PrefilterMips &set_out_cube(Texture out_texture);

        virtual void executeCommand() const override;

    private:
        String path_;
        Texture out_cube_;
    };
}

#endif // __LEEP_RENDER_COMMANDS_EQUIRECTANGULAR_TO_CUBEMAP_H__
