// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_COMMANDS_USE_PBR_MATERIAL_H__
#define __LEEP_RENDER_COMMANDS_USE_PBR_MATERIAL_H__ 1

#include "render/display-list-command.h"
#include "render/texture.h"
#include "glm/mat4x4.hpp"
 
namespace leep
{
    // Here will go all the uniform values that not change
    // with each entity, like camera and lights
    struct PbrSceneData
    {
        glm::mat4x4 view_projection_;
		glm::vec3 camera_position_;
		float padding_;
		glm::vec4 light_direction_intensity_;
    };

    class UsePbrMaterial: public DLComm
    {
    public:
        UsePbrMaterial();
        UsePbrMaterial(const UsePbrMaterial&) = delete;
        UsePbrMaterial(UsePbrMaterial&&) = delete;
        ~UsePbrMaterial();

        UsePbrMaterial& set_scene_data(const PbrSceneData &data);
        UsePbrMaterial& set_irradiance_map(Texture irradiance);
        UsePbrMaterial& set_prefilter_map(Texture prefilter);
        UsePbrMaterial& set_lut_map(Texture lut);

        virtual void executeCommand() const override;

    private:
        // I need a copy of the state of the data when the command
        // is added to the list. If I use a reference or pointer 
        // the data could change before the render thread executes
        // the command.
        PbrSceneData data_;
        Texture irr_;
        Texture pref_;
        Texture lut_;
    };
}

#endif // __LEEP_RENDER_COMMANDS_USE_PBR_MATERIAL_H__
