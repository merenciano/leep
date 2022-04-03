#ifndef __THE_RENDER_MATERIAL_H__
#define __THE_RENDER_MATERIAL_H__

#include "core/Cdefinitions.h"

typedef s32 THE_Texture

typedef enum
{
	THE_MT_BASIC = 0,
	THE_MT_FULL_SCREEN_IMAGE,
	THE_MT_SKYBOX,
	THE_MT_EQUIREC_TO_CUBE,
	THE_MT_PREFILTER_ENV,
	THE_MT_LUT_GEN,
	// USER DEFINED MATERIALS START FROM HERE
	THE_MT_PBR,
	THE_MT_VOXMAT,
	THE_MT_MAX,
	// MT_NONE after MT_MAX because MT_MAX is used set the
	// size of a valid materials array
	THE_MT_NONE
} THE_MaterialType;

typedef struct  
{
        float *data_; 
        THE_Texture *tex_;
        MaterialType type_; // Acts as a handle
        s32 dcount_;
        s32 tcount_;
        s32 cube_start_;
} THE_Material;

typedef struct
{
	float model_[16];
	glm::vec3 color;
	float use_albedo_map;
	float use_pbr_maps;
	float tiling_x;
	float tiling_y;
	float padding;
	float roughness;
	float metallic;
	float normal_map_intensity;
	float paddingg;
} THE_PbrData;

typedef struct
{
	glm::mat4x4 view_projection;
	glm::vec3 camera_position;
	float padding;
	glm::vec4 light_direction_intensity;
} THE_PbrSceneData;

typedef struct
{
	float model[16];
	glm::vec3 color;
	float roughness;
	float metallic;
	float padding[3];
} THE_VoxMatData;

typedef struct
{
	glm::mat4 vp;
	glm::vec3 camera_position;
	float padding;
	glm::vec4 light_direction_intensity;
} THE_VoxMatSceneData;

typedef struct
{
	glm::mat4 vp;
} THE_EquirecToCubeData;

typedef struct
{
	glm::mat4 vp;
	float roughness;
	float padding[3];
} THE_PrefilterEnvData;

#endif