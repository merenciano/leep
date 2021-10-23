// Lucas Merenciano Martinez <lucasmermar@gmail.com>

// This command can draw instanced, to do so set_instanced_opts
// must be called before, otherwise only one instance will be drawn.
// The buffer will store 3 floats per instance (pos X, pos Y and pos Z)

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

        Draw& set_geometry(Geometry geometry);
        Draw& set_material(Material material);
        Draw& set_instanced_opts(uint32_t count, Buffer inst_attr);

        virtual void executeCommand() const override;

    private:
        Geometry geometry_; 
        Material material_;

        Buffer inst_attr_;
        int32_t inst_count_;
    };
}

#endif // __LEEP_RENDER_COMMANDS_DRAW_H__
