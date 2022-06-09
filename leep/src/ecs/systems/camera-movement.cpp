#include "camera-movement.h"

#include "core/common-defs.h"
#include "core/input.h"
#include "core/manager.h"
#include "core/window.h"
#include "ecs/components/ltransform.h"
#include "render/Crenderer.h"
#include "render/Ccamera.h"

#include <iostream>

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
        if (GM.input().isButtonDown(Button::MOUSE_RIGHT))
        {
            mouse_down_pos[0] = GM.input().mouseX();
            mouse_down_pos[1] = GM.input().mouseY();
        }

        if (GM.input().isButtonPressed(Button::MOUSE_RIGHT))
        {
            float mouse_offset[2] = {
                GM.input().mouseX() - mouse_down_pos[0],
                mouse_down_pos[1] - GM.input().mouseY()
            }; // Y axis inverted

            mouse_offset[0] *= sensibility_;
            mouse_offset[1] *= sensibility_;

	    
	    //tr.transform = smat4_multiply(smat4_rotation_y(-mouse_offset[0]), tr.transform);
	    //tr.transform = smat4_multiply(smat4_rotation_x(mouse_offset[1]), tr.transform);
	    tr.transform = LTransform::rotateYWorld(tr.transform, -mouse_offset[0]);
            //tr.rotateYWorld(-mouse_offset[0]);
            //tr.transform_ = glm::rotate(tr.transform_, mouse_offset[1], glm::vec3(1.0f, 0.0f, 0.0f));

            mouse_down_pos[0] = GM.input().mouseX();
            mouse_down_pos[1] = GM.input().mouseY();
        }

        // Position
        if (GM.input().isButtonPressed(Button::UP))
        {
	    tr.transform = smat4_translate(tr.transform, svec3(0.0f, 0.0f, -speed));
        }

        if (GM.input().isButtonPressed(Button::DOWN))
        {
	    tr.transform = smat4_translate(tr.transform, svec3(0.0f, 0.0f, speed));
        }

        if (GM.input().isButtonPressed(Button::LEFT))
        {
	    tr.transform = smat4_translate(tr.transform, svec3(-speed, 0.0f, 0.0f));
        }

        if (GM.input().isButtonPressed(Button::RIGHT))
        {
	    tr.transform = smat4_translate(tr.transform, svec3(speed, 0.0f, 0.0f));
        }

        if (GM.input().isButtonPressed(Button::B1))
        {
	    tr.transform = smat4_translate(tr.transform, svec3(0.0f, speed, 0.0f));
        }

        if (GM.input().isButtonPressed(Button::B4))
        {
	    tr.transform = smat4_translate(tr.transform, svec3(0.0f, -speed, 0.0f));
        }

        if (GM.input().scroll() != 0.0f)
        {
            fov -= GM.input().scroll() * scroll_sensibility;
            fov = glm::clamp(fov, 1.0f, 120.0f);
	    camera.proj_mat = smat4_perspective(to_radians(fov), (float)GM.window().width() / GM.window().height(), 0.1f, camera.far_value);
        }
	

	std::cout << "########################################################" << std::endl;
	std::cout << "########################################################" << std::endl;
	std::cout << tr.transform.m11 << tr.transform.m12 << tr.transform.m13 << tr.transform.m14 << std::endl;
	std::cout << tr.transform.m21 << tr.transform.m22 << tr.transform.m23 << tr.transform.m24 << std::endl;
	std::cout << tr.transform.m31 << tr.transform.m32 << tr.transform.m33 << tr.transform.m34 << std::endl;
	std::cout << tr.transform.m41 << tr.transform.m42 << tr.transform.m43 << tr.transform.m44 << std::endl;

	camera.view_mat = smat4_inverse(tr.transform);
    }
}