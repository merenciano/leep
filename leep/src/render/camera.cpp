#include "camera.h"
#include "core/common-defs.h"
#include "core/manager.h"
#include "core/window.h"

namespace leep
{
    Camera::Camera()
    {
        view_matrix_ = glm::mat4(1.0f);
        projection_matrix_ = glm::mat4(1.0f);
        fov_ = 70.0f;
        far_ = 100.0f;
    }

    // Sorry... far is #defined...
    Camera::Camera(float fov, float farval)
    {
        view_matrix_ = glm::mat4(1.0f);
        projection_matrix_ = glm::mat4(1.0f);
        fov_ = fov;
        far_ = farval;
    }

    Camera::~Camera()
    {
    }

    void Camera::init()
    {
        init(fov_, far_);
    }

    void Camera::init(float fov, float farval)
    {
        fov_ = fov;
        far_ = farval;
        view_matrix_ = glm::mat4(1.0f);
        projection_matrix_ = glm::perspective(glm::radians(fov), (float)GM.window().width() / GM.window().height(), 0.01f, farval);
        fb_.create(GM.window().fwidth(), GM.window().fheight(), true, true);
    }

    void Camera::init(float width, float height, bool is_light)
    {
        view_matrix_ = glm::mat4(1.0f);
        projection_matrix_ = glm::perspective(glm::radians(fov_), (float)GM.window().fwidth() / GM.window().fheight(), 0.1f, far_);
        if (is_light)
            fb_.create(width, height, false, true);
        else
            fb_.create(width, height, true, true);
    }

    void Camera::set_view(const glm::mat4 &view)
    {
        view_matrix_ = view;
    }

    void Camera::set_projection(float fov, float aspect, float near_value, float far_value)
    {
        far_ = far_value;
        fov_ = fov;
        projection_matrix_ = glm::perspective(glm::radians(fov), aspect, near_value, far_value);
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

    float Camera::fov() const
    {
        return fov_;
    }

    float Camera::farval() const
    {
        return far_;
    }
}
