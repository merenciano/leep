#include "camera.h"
#include "core/io.h"
#include "core/manager.h"
#include "ecs/transform.h"
#include "internalresources.h"

float rot_x = 0.0f;
float rot_y = 0.0f;

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

void THE_CameraMovementSystem(THE_Camera *cam)
{
	static const float ksensibility = 1.0f / 1000.0f;
	static const float kspeed = 10.0f;
	static const float kscroll_sensibility = 1.0f;

	static float mouse_down_pos[2] = { 0.0, 0.0 };
	static float fov = 70.0f;

	float speed = kspeed * THE_DeltaTime();

	//struct mat4 tr = smat4_inverse(cam->view_mat);
	struct mat4 tr = smat4_identity();

	// Rotation
	if (THE_InputIsButtonDown(THE_MOUSE_RIGHT)) {
		mouse_down_pos[0] = THE_InputGetMouseX();
		mouse_down_pos[1] = THE_InputGetMouseY();
	}

	if (THE_InputIsButtonPressed(THE_MOUSE_RIGHT))
	{
		float mouse_offset[2] = {
			THE_InputGetMouseX() - mouse_down_pos[0],
			mouse_down_pos[1] - THE_InputGetMouseY()
		}; // Y axis inverted

		mouse_offset[0] *= ksensibility;
		mouse_offset[1] *= ksensibility;

		rot_x += mouse_offset[1];
		rot_y -= mouse_offset[0];

		tr = THE_TransformRotateYWorld(tr, -mouse_offset[0]);
		tr = smat4_multiply(smat4_rotation_x(mouse_offset[1]), tr);
		//tr = THE_TransformRotateInPlace(tr, mouse_offset[1], -mouse_offset[0]);

		mouse_down_pos[0] = THE_InputGetMouseX();
		mouse_down_pos[1] = THE_InputGetMouseY();
	}

	// Position
	if (THE_InputIsButtonPressed(THE_KEY_UP))
	{
		//tr = THE_TransformTranslateLocal(tr, svec3(0.0f, 0.0f, -speed));
		tr = smat4_translate(tr, svec3(0.0f, 0.0f, -speed));
	}

	if (THE_InputIsButtonPressed(THE_KEY_DOWN))
	{
		tr = smat4_translate(tr, svec3(0.0f, 0.0f, speed));
	}

	if (THE_InputIsButtonPressed(THE_KEY_LEFT))
	{
		tr = smat4_translate(tr, svec3(-speed, 0.0f, 0.0f));
	}

	if (THE_InputIsButtonPressed(THE_KEY_RIGHT))
	{
		tr = smat4_translate(tr, svec3(speed, 0.0f, 0.0f));
	}

	if (THE_InputIsButtonPressed(THE_KEY_1))
	{
		tr = smat4_translate(tr, svec3(0.0f, speed, 0.0f));
	}

	if (THE_InputIsButtonPressed(THE_KEY_4))
	{
		tr = smat4_translate(tr, svec3(0.0f, -speed, 0.0f));
	}

	tr = smat4_multiply(smat4_inverse(cam->view_mat), tr);

	// Zoom
	if (THE_InputGetScroll() != 0.0f)
	{
		fov -= THE_InputGetScroll() * kscroll_sensibility;
		fov = clampf(fov, 1.0f, 120.0f);
		cam->proj_mat = smat4_perspective(to_radians(fov), (float)THE_WindowGetWidth() / (float)THE_WindowGetHeight(), 0.1f, camera.far_value);
	}

	cam->view_mat = smat4_inverse(tr);
}