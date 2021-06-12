// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_MATERIAL_TYPES_H__
#define __LEEP_RENDER_MATERIAL_TYPES_H__

#include "core/common-defs.h"

namespace leep
{
    // Not enum class because its values are used as array indexes for the renderer materials
    enum MaterialType
    {
        MT_BASIC = 0,
        MT_FULL_SCREEN_IMAGE = 1,
        MT_SKYBOX = 2,
        MT_EQUIREC_TO_CUBE = 3,
        MT_PREFILTER_ENV = 4,
        MT_LUT_GEN = 5,

        // USER DEFINED MATERIALS START FROM HERE
        MT_PBR,
        MT_VOXMAT,

        MT_MAX,
        // MT_NONE after MT_MAX because MT_MAX is used set the
        // size of a valid materials array
        MT_NONE
    };

    struct PbrData
    {
		float model_[16];
		glm::vec3 color_;
		float use_albedo_map_;
        float use_pbr_maps_;
        float tiling_x_;
        float tiling_y_;
		float padding;
		float roughness_;
		float metallic_;
        float normal_map_intensity_;
		float paddingg;
    };

    struct PbrSceneData
    {
        glm::mat4x4 view_projection_;
		glm::vec3 camera_position_;
		float padding_;
		glm::vec4 light_direction_intensity_;
    };

    struct VoxMatData
    {
		float model_[16];
		glm::vec3 color_;
		float roughness_;
		float metallic_;
        float padding[3];
    };

    struct VoxMatSceneData
    {
        glm::mat4 vp_;
		glm::vec3 camera_position_;
		float padding_;
		glm::vec4 light_direction_intensity_;
    };

    struct EquirecToCubeData
    {
        glm::mat4 vp_;
    };

    struct PrefilterEnvData
    {
        glm::mat4 vp_;
        float roughness_;
        float padding[3];
    };
}
#endif // __LEEP_RENDER_MATERIAL_TYPES_H__