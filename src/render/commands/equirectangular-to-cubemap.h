// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_COMMANDS_EQUIRECTANGULAR_TO_CUBEMAP_H__
#define __LEEP_RENDER_COMMANDS_EQUIRECTANGULAR_TO_CUBEMAP_H__

#include "render/display-list-command.h"
#include "render/texture.h"

namespace leep
{
    class EquirectangularToCubemap : public DisplayListCommand
    {
    public:
        EquirectangularToCubemap();
        ~EquirectangularToCubemap();

        EquirectangularToCubemap &set_in_path(std::string in_path);
        EquirectangularToCubemap &set_out_cube(Texture out_texture);
        EquirectangularToCubemap &set_out_prefilter(Texture out_texture);

        virtual void executeCommand() const override;

    private:
        std::string path_;
        Texture out_cube_;
        Texture out_pref_;
    };
}

#endif // __LEEP_RENDER_COMMANDS_EQUIRECTANGULAR_TO_CUBEMAP_H__
