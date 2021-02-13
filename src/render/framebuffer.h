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

		void create(float width, float height, bool color = true, bool depth = true);
		void release();

		void set_color_texture(Texture color);
		void set_depth_texture(Texture depth);
		Texture color() const;
		Texture depth() const;
		int32_t id() const;

	private:
		int32_t handler_;
	};
}

#endif // __LEEP_RENDER_FRAMEBUFFER_H__