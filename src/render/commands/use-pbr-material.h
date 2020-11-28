// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_COMMANDS_USE_PBR_MATERIAL_H__
#define __LEEP_RENDER_COMMANDS_USE_PBR_MATERIAL_H__ 1

#include "render/display-list-command.h"
#include "glm/mat4x4.hpp"
 
namespace leep
{
    // Here will go all the uniform values that not change
    // with each entity, like camera and lights
    struct PbrSceneData
    {
        glm::mat4x4 view_projection;
    };

    class UsePbrMaterial: public DisplayListCommand
    {
    public:
        UsePbrMaterial();
        UsePbrMaterial(const UsePbrMaterial&) = delete;
        UsePbrMaterial(UsePbrMaterial&&) = delete;
        ~UsePbrMaterial();

        UsePbrMaterial& set_scene_data(const PbrSceneData &data);

        virtual void executeCommand() const override;

    private:
        // I need a copy of the state of the data when the command
        // is added to the list. If I use a reference or pointer 
        // the data could change before the render thread executes
        // the command.
        PbrSceneData data_;
    };
};

#endif // __LEEP_RENDER_COMMANDS_USE_PBR_MATERIAL_H__
