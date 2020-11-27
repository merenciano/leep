// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_COMMANDS_INIT_MATERIAL_H__
#define __LEEP_RENDER_COMMANDS_INIT_MATERIAL_H__ 1

#include "render/display-list-command.h"
#include "render/material.h"

namespace leep
{
    class InitMaterial : public DisplayListCommand
    {
    public:
        InitMaterial();
        ~InitMaterial();

        InitMaterial& set_material(MaterialType material);

        virtual void executeCommand() const override;
    private:
        MaterialType material_;

    };
}

#endif // __LEEP_RENDER_COMMANDS_INIT_MATERIAL_H__
