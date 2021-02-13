// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_MATERIAL_TYPES_H__
#define __LEEP_RENDER_MATERIAL_TYPES_H__ 1

#include "glm/mat4x4.hpp"

namespace leep
{
    // Not enum class because its values are used as array indexes for the renderer materials
    enum MaterialType
    {
        MT_PBR = 0,
        MT_FULL_SCREEN_IMAGE = 1,
        MT_SKYBOX = 2,

        MT_MAX,
        MT_NONE
    };

    struct PbrData
    {
	private:
		// The model matrix is setted with set_model method of the material
		float model_[16];
	public:
		glm::vec3 color_;
		float use_albedo_map_;
		float paddinggg;
        float tiling_x_;
        float tiling_y_;
		float padding;
		float roughness_;
		float metallic_;
		float reflectance_;
		float paddingg;
    };
}
#endif // __LEEP_RENDER_MATERIAL_TYPES_H__