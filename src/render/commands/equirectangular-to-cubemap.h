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

        EquirectangularToCubemap &set_in_equirect(Texture t);
        EquirectangularToCubemap &set_out_cube(Texture t);

        virtual void executeCommand() const override;

    private:
        Texture in_equi_;
        Texture out_cube_;
    };
}

#endif // __LEEP_RENDER_COMMANDS_EQUIRECTANGULAR_TO_CUBEMAP_H__
