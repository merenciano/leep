// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_COMMANDS_DRAW_H__
#define __LEEP_RENDER_COMMANDS_DRAW_H__

#include "render/display-list-command.h"
#include "render/geometry.h"
#include "render/material.h"

namespace leep
{
    class Draw : public DLComm
    {
    public:
        Draw();
        ~Draw();

        Draw& set_geometry(const Geometry &geometry);
        Draw& set_material(const Material &material);
        Draw& set_instanced_opts(uint32_t count, Buffer inst_attr);

        virtual void executeCommand() const override;

    private:
        Geometry geometry_; 
        Material material_;

        Buffer inst_attr_;
        uint32_t inst_count_;
    };
}

#endif // __LEEP_RENDER_COMMANDS_DRAW_H__
