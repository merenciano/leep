/*
    Provisional camera.
    TODO: Think a better way to implement camera in the future.
    Maybe a component in a entity.
*/

#ifndef __LEEP_RENDER_CAMERA_H__
#define __LEEP_RENDER_CAMERA_H__

#include "render/framebuffer.h"
#include "core/common-defs.h"

namespace leep
{
    class Camera
    {
    public:
        Camera();
        Camera(float fov, float far);
        ~Camera();

        // Framebuffer size will be the screen size and will not be used for shadowmaps
        void init();
        void init(float fov, float farval);
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
        float fov() const;
        float farval() const;

        Framebuffer framebuffer() const;

    private:
        glm::mat4 view_matrix_;
        glm::mat4 projection_matrix_;
        Framebuffer fb_;
        float far_;
        float fov_;
    };
}

#endif // __LEEP_RENDER_CAMERA_H__
