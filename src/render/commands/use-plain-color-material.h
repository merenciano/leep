// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_COMMANDS_USE_PLAIN_COLOR_MATERIAL_H__
#define __LEEP_RENDER_COMMANDS_USE_PLAIN_COLOR_MATERIAL_H__ 1

#include "render/display-list-command.h"
#include "glm/mat4x4.hpp"
 

namespace leep 
{
    struct PlainColorSceneData
    {
        glm::mat4x4 view_projection;
    };
    
    class UsePlainColorMaterial: public DisplayListCommand
    {
    public:
        UsePlainColorMaterial();
        UsePlainColorMaterial(const UsePlainColorMaterial&) = delete;
        UsePlainColorMaterial(UsePlainColorMaterial&&) = delete;
        ~UsePlainColorMaterial();

        UsePlainColorMaterial& set_scene_data(const PlainColorSceneData &scene_data);

        virtual void executeCommand() const override;
    
    private:
        PlainColorSceneData data_;
    };
};

#endif // __LEEP_RENDER_COMMANDS_USE_PLAIN_COLOR_MATERIAL_H__
