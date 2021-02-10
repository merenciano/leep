// Lucas Merenciano Martinez  <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_INTERNAL_FRAMEBUFFER_H__
#define __LEEP_RENDER_INTERNAL_FRAMEBUFFER_H__

#include <stdint.h>

namespace leep
{
	struct InternalFramebuffer
	{
		int32_t internal_id_;
		int32_t version_;
		float width_;
		float height_;
		bool color_;
		bool depth_;
	};
}

#endif // __LEEP_RENDER_INTERNAL_FRAMEBUFFER_H__