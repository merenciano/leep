// Lucas Merenciano Martinez  <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_FRAMEBUFFER_H__
#define __LEEP_RENDER_FRAMEBUFFER_H__

#include "render/texture.h"

namespace leep
{
	class Framebuffer
	{
	public:
		Framebuffer();
		Framebuffer(const Framebuffer &other) = default;
		~Framebuffer();

		Framebuffer& operator=(const Framebuffer &other) = default;

        /**
         * @brief Creates the framebuffer handler
         * 
         * @param width Width of the textures if color or depth are true
         * @param height Height of the textures if color or depth are true
         * @param color Create and attach a color texture
         * @param depth Create and attach a depth texture
         */
		void create(float width, float height,
                    bool color = true, bool depth = true);

        /**
         * @brief Release framebuffer
         * 
         * Invalidates the handle and marks the resource on the gpu for deletion
         * 
         */
		void release();

        void set_color_texture(Texture color);
        void set_depth_texture(Texture depth);
        Texture color() const;
        Texture depth() const;
        int32_t handle() const;

	private:
		int32_t handle_;
	};
}

#endif // __LEEP_RENDER_FRAMEBUFFER_H__