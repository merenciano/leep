#ifndef __MOTORET_RENDER_USE_PBR_MATERIAL_COMMAND_H__
#define __MOTORET_RENDER_USE_PBR_MATERIAL_COMMAND_H__ 1

#include "render/display-list-command.h"
#include "glm/mat4x4.hpp"
 

namespace motoret
{
    // Here will go all the uniform values that not change
    // with each entity, like camera and lights
    struct PbrSceneData
    {
        glm::mat4x4 view_projection_;
    };

    class UsePbrMaterial: public DisplayListCommand
    {
    public:
        UsePbrMaterial();
        UsePbrMaterial(const UsePbrMaterial&) = delete;
        UsePbrMaterial(UsePbrMaterial&&) = delete;
        ~UsePbrMaterial();

        UsePbrMaterial& set_data(const PbrSceneData &data);

        virtual void executeCommand() const override;

    private:
        // I need a copy of the state of the data when the command
        // is added to the list. If I use a reference or pointer 
        // the data could change before the render thread executes
        // the command.
        PbrSceneData data_;
    };
};

#endif // __MOTORET_RENDER_USE_PBR_MATERIAL_COMMAND_H__