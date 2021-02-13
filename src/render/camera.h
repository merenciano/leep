/*
    Provisional camera.
    Think a better way to implement camera in the future.
    Maybe a component in a entity.
*/

#ifndef __LEEP_RENDER_CAMERA_H__
#define __LEEP_RENDER_CAMERA_H__ 1

#include "render/framebuffer.h"
#include "glm/mat4x4.hpp"

namespace leep
{
    class Camera
    {
    public:
        Camera();
        ~Camera();

        // Framebuffer size will be the screen size and will not be used for shadowmaps
        void init();
        // Framebuffer size and set if will be used for shadowmaps (only depth on the fb)
        void init(float width, float height, bool is_light);
        void set_view(const glm::mat4 &view);
        void set_projection(float fovy, float aspect, float near_value, float far_value);
        const glm::mat4& view() const;
        const glm::mat4& projection() const;
        glm::mat4 view_projection() const;
        // Matrix without the translate values.
        // Result from projection * vec4(vec3(view));
        // Used for the skybox
        glm::mat4 static_view_projection() const;
		glm::vec3 position() const;

        Framebuffer framebuffer() const;

    private:
        glm::mat4 view_matrix_;
        glm::mat4 projection_matrix_;
        Framebuffer fb_;
    };
}

#endif // __LEEP_RENDER_CAMERA_H__
