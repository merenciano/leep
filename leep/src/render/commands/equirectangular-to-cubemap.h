
#ifndef __LEEP_RENDER_COMMANDS_EQUIRECTANGULAR_TO_CUBEMAP_H__
#define __LEEP_RENDER_COMMANDS_EQUIRECTANGULAR_TO_CUBEMAP_H__

#include "render/display-list-command.h"
#include "render/texture.h"
#include "core/string.h"

namespace leep
{
    class EquirectangularToCubemap : public DLComm
    {
    public:
        EquirectangularToCubemap();
        ~EquirectangularToCubemap();

        EquirectangularToCubemap &set_in_path(String in_path);
        EquirectangularToCubemap &set_out_cube(Texture out_texture);
        EquirectangularToCubemap &set_out_prefilter(Texture out_texture);
        EquirectangularToCubemap &set_out_lut(Texture out_texture);

        virtual void executeCommand() const override;

    private:
        String path_;
        Texture out_cube_;
        Texture out_pref_;
        Texture out_lut_;
    };
}

#endif // __LEEP_RENDER_COMMANDS_EQUIRECTANGULAR_TO_CUBEMAP_H__
