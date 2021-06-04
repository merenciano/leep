// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_COMMANDS_USE_SKYBOX_MATERIAL_H__
#define __LEEP_RENDER_COMMANDS_USE_SKYBOX_MATERIAL_H__

#include "core/common-defs.h"
#include "render/display-list-command.h"
 
namespace leep
{
    class UseSkyboxMaterial: public DLComm
    {
    public:
        UseSkyboxMaterial();
        UseSkyboxMaterial(const UseSkyboxMaterial&) = delete;
        UseSkyboxMaterial(UseSkyboxMaterial&&) = delete;
        ~UseSkyboxMaterial();

        virtual void executeCommand() const override;
    };
}

#endif // __LEEP_RENDER_COMMANDS_USE_SKYBOX_MATERIAL_H__
