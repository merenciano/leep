// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_COMMANDS_DRAW_SKYBOX_H__
#define __LEEP_RENDER_COMMANDS_DRAW_SKYBOX_H__ 1

#include "render/display-list-command.h"
#include "render/material.h"

namespace leep
{
    class DrawSkybox : public DLComm
    {
    public:
        DrawSkybox();
        ~DrawSkybox();

        DrawSkybox& set_cubemap(const Texture &geometry);

    protected:
        virtual void executeCommand() const override;

    private:
        Material material_;
    };

}

#endif // __LEEP_RENDER_COMMANDS_DRAW_SKYBOX_H__
