#include "camera.h"
#include "core/common-defs.h"
#include "core/manager.h"
#include "core/window.h"

#include "glm/gtc/matrix_transform.hpp"

namespace leep
{
    Camera::Camera()
    {
        view_matrix_ = glm::mat4(1.0f);
        projection_matrix_ = glm::mat4(1.0f);
    }

    Camera::~Camera()
    {
    }

    void Camera::init()
    {
        view_matrix_ = glm::mat4(1.0f);
        projection_matrix_ = glm::perspective(glm::radians(70.0f), (float)GM.window().width() / GM.window().height(), 0.01f, 80.0f);
        fb_.create(GM.window().fwidth(), GM.window().fheight(), true, true);
    }

    void Camera::init(float width, float height, bool is_light)
    {
        view_matrix_ = glm::mat4(1.0f);
        projection_matrix_ = glm::perspective(glm::radians(70.0f), (float)GM.window().fwidth() / GM.window().fheight(), 0.1f, 80.0f);
        if (is_light)
            fb_.create(width, height, false, true);
        else
            fb_.create(width, height, true, true);
    }

    void Camera::set_view(const glm::mat4 &view)
    {
        view_matrix_ = view;
    }

    void Camera::set_projection(float fovy_rad, float aspect, float near_value, float far_value)
    {
        projection_matrix_ = glm::perspective(fovy_rad, aspect, near_value, far_value);
    }

    const glm::mat4& Camera::view() const
    {
        return view_matrix_;
    }

    const glm::mat4& Camera::projection() const
    {
        return projection_matrix_;
    }

    glm::mat4 Camera::view_projection() const
    {
        return projection_matrix_ * view_matrix_;
    }

    glm::mat4 Camera::static_view_projection() const
    {
        return projection_matrix_ * glm::mat4(glm::mat3(view_matrix_));
    }

	glm::vec3 Camera::position() const
	{
		glm::mat4 inv = glm::inverse(view_matrix_);
		return glm::vec3(inv[3][0], inv[3][1], inv[3][2]);
	}

    Framebuffer Camera::framebuffer() const
    {
        return fb_;
    }
}
