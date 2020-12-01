/*
    Provisional camera.
    Think a better way to implement camera in the future.
    Maybe a component in a entity.
*/

#ifndef __LEEP_RENDER_CAMERA_H__
#define __LEEP_RENDER_CAMERA_H__ 1

#include "glm/mat4x4.hpp"

namespace leep
{
    class Camera
    {
    public:
        Camera();
        ~Camera();

        void set_view(const glm::mat4 &view);
        void set_perspective(float fovy, float aspect, float near, float far);
        const glm::mat4& view() const;
        const glm::mat4& projection() const;
        glm::mat4 view_projection() const;

    private:
        glm::mat4 view_matrix_;
        glm::mat4 projection_matrix_;
    };
}

#endif // __LEEP_RENDER_CAMERA_H__
