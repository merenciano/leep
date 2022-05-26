#ifndef __THE_RENDER_CAMERA_H__
#define __THE_RENDER_CAMERA_H__

#include "Crenderer.h"
#include "mathc.h"

typedef struct {
	struct mat4 view_mat;
	struct mat4 proj_mat;
	float far;
	float fov;
	THE_Framebuffer fb;
} THE_Camera;

void THE_CameraInit(THE_Camera* cam, float fov, float far, u32 width, u32 height, u8 is_light);
struct vec3 THE_CameraPosition(THE_Camera *cam);

/* Matrix without the translate values.
 * Result from projection * vec4(vec3(view));
 * Used for the skybox
 */
struct mat4 THE_CameraStaticViewProjection(THE_Camera *cam);

#endif
