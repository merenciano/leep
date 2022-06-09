#include "Ccamera.h"
#include "Cinternalresources.h"

void THE_CameraInit(THE_Camera *cam, float fov, float far, u32 width, u32 height, u8 is_light)
{
	cam->fov = fov;
	cam->far_value = far;
	cam->view_mat = smat4_identity();
	cam->proj_mat = smat4_perspective_fov(to_radians(fov), (float)width, (float)height, 0.01f, far);
	cam->fb = THE_CreateFramebuffer(width, height, !is_light, true);
}

struct mat4 THE_CameraStaticViewProjection(THE_Camera *cam)
{
	struct mat4 static_view = smat4(
		cam->view_mat.m11, cam->view_mat.m12, cam->view_mat.m13, 0.0f,
		cam->view_mat.m21, cam->view_mat.m22, cam->view_mat.m23, 0.0f,
		cam->view_mat.m31, cam->view_mat.m32, cam->view_mat.m33, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f );

	return smat4_multiply(cam->proj_mat, static_view);
}

struct vec3 THE_CameraPosition(THE_Camera *cam)
{
	struct mat4 inv = smat4_inverse(cam->view_mat);
	return svec3(inv.m14, inv.m24, inv.m34);
}

THE_Texture THE_CameraOutputColorTexture(THE_Camera *cam)
{
	return framebuffers[cam->fb].color_tex;
}