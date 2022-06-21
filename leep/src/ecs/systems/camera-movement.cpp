#include "camera-movement.h"

#include "core/io.h"
#include "core/common-defs.h"
#include "core/manager.h"
#include "ecs/components/ltransform.h"
#include "render/Crenderer.h"
#include "render/Ccamera.h"

namespace leep
{
    CameraMovement::CameraMovement()
    {
        sensibility_ = 1.0f / 1000.0f;
        speed_ = 1.0f;
    }

    CameraMovement::CameraMovement(float sensibility, float speed)
    {
        sensibility_ = sensibility / 1000.0f;
        speed_ = speed;
    }

    void CameraMovement::executeSystem() const
    {
        static float mouse_down_pos[2] = { 0.0, 0.0 };
        static float fov = 70.0f;
        //float sensibility = sensibility_;
        float speed = speed_ * GM.delta_time();
        float scroll_sensibility = 1.0f;
        LTransform tr;
	tr.transform = smat4_inverse(camera.view_mat);

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

            mouse_offset[0] *= sensibility_;
            mouse_offset[1] *= sensibility_;

	    tr.transform = LTransform::rotateYWorld(tr.transform, -mouse_offset[0]);
	    tr.transform = smat4_multiply(tr.transform, smat4_rotation_x(mouse_offset[1]));

            mouse_down_pos[0] = THE_InputGetMouseX();
            mouse_down_pos[1] = THE_InputGetMouseY();
        }

        // Position
        if (THE_InputIsButtonPressed(THE_KEY_UP))
        {
	    tr.transform = smat4_translate(tr.transform, svec3(0.0f, 0.0f, -speed));
        }

        if (THE_InputIsButtonPressed(THE_KEY_DOWN))
        {
	    tr.transform = smat4_translate(tr.transform, svec3(0.0f, 0.0f, speed));
        }

        if (THE_InputIsButtonPressed(THE_KEY_LEFT))
        {
	    tr.transform = smat4_translate(tr.transform, svec3(-speed, 0.0f, 0.0f));
        }

        if (THE_InputIsButtonPressed(THE_KEY_RIGHT))
        {
	    tr.transform = smat4_translate(tr.transform, svec3(speed, 0.0f, 0.0f));
        }

        if (THE_InputIsButtonPressed(THE_KEY_1))
        {
	    tr.transform = smat4_translate(tr.transform, svec3(0.0f, speed, 0.0f));
        }

        if (THE_InputIsButtonPressed(THE_KEY_4))
        {
	    tr.transform = smat4_translate(tr.transform, svec3(0.0f, -speed, 0.0f));
        }

        if (THE_InputGetScroll() != 0.0f)
        {
            fov -= THE_InputGetScroll() * scroll_sensibility;
            fov = glm::clamp(fov, 1.0f, 120.0f);
	    camera.proj_mat = smat4_perspective(to_radians(fov), (float)THE_WindowGetWidth() / (float)THE_WindowGetHeight(), 0.1f, camera.far_value);
        }

	camera.view_mat = smat4_inverse(tr.transform);
    }
}