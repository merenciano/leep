#ifndef __MOTORET_RENDER_DRAW_COMMAND_H__
#define __MOTORET_RENDER_DRAW_COMMAND_H__ 1

#include "render/display-list-command.h"
#include "render/geometry.h"

namespace motoret
{
    class Draw : public DisplayListCommand
    {
    public:
        Draw();
        ~Draw();

        virtual void executeCommand() const override;

    private:
        Geometry geometry_; 
    };

}

#endif // __MOTORET_RENDER_DRAW_COMMAND_H__
