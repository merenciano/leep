#include "camera.h"
#include "core/common-defs.h"
#include "core/manager.h"

#include "glm/gtc/matrix_transform.hpp"

namespace leep
{
    Camera::Camera()
    {
    }

    Camera::~Camera()
    {

    }

    void Camera::init()
    {
        view_matrix_ = glm::mat4(1.0f);
        projection_matrix_ = glm::perspective(glm::radians(70.0f), (float)GM.window().width() / GM.window().height(), 0.1f, 80.0f);
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
}
