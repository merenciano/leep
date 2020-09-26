#ifndef __MOTORET_RENDER_COMMANDS_USE_PLAIN_COLOR_MATERIAL_H__
#define __MOTORET_RENDER_COMMANDS_USE_PLAIN_COLOR_MATERIAL_H__ 1

#include "render/display-list-command.h"
#include "glm/mat4x4.hpp"
 

namespace motoret
{
    class UsePlainColorMaterial: public DisplayListCommand
    {
    public:
        UsePlainColorMaterial();
        UsePlainColorMaterial(const UsePlainColorMaterial&) = delete;
        UsePlainColorMaterial(UsePlainColorMaterial&&) = delete;
        ~UsePlainColorMaterial();

        virtual void executeCommand() const override;
    };
};

#endif // __MOTORET_RENDER_COMMANDS_USE_PLAIN_COLOR_MATERIAL_H__
