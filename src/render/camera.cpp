#include "camera.h"

#include "glm/gtc/matrix_transform.hpp"

namespace leep
{
    Camera::Camera()
    {
        view_matrix_ = glm::mat4(1.0f);
        projection_matrix_ = glm::perspective(glm::radians(70.0f), 1280.0f / 720.0f, 0.1f, 50.0f);
    }

    Camera::~Camera()
    {

    }

    void Camera::set_view(const glm::mat4 &view)
    {
        view_matrix_ = view;
    }

    void Camera::set_perspective(float fovy_rad, float aspect, float near, float far)
    {
        glm::perspective(fovy_rad, aspect, near, far);
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
        return view_matrix_ * projection_matrix_;
    }
}
