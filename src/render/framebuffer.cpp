#include "framebuffer.h"
#include "core/common-defs.h"
#include "core/manager.h"
#include "core/window.h"
#include "render/renderer.h"

namespace leep
{
	Framebuffer::Framebuffer()
	{
		handler_ = -1;
	}

	Framebuffer::~Framebuffer()
	{

	}
	
	void Framebuffer::create(float width, float height, bool depth)
	{
		LEEP_ASSERT((width > 1.0f && height > 1.0f) ||
					(width <= 1.0f && height <= 1.0f),
					"CreateFramebuffer: Or absolute or relative, make a decision!");
		LEEP_ASSERT(handler_ == ConstantValues::UNINITIALIZED_HANDLER, "This framebuffer is already created");
		Renderer &r = GM.renderer();
		if (!r.aviable_fb_pos_.empty())
		{
			handler_ = r.aviable_fb_pos_.front();
			r.aviable_fb_pos_.pop_front();
		}
		else
		{
			InternalFramebuffer ifb;
			r.framebuffers_.push_back(ifb);
			handler_ = (int32_t)r.framebuffers_.size() - 1;
		}

		color_texture_.createEmpty(width, height);
		depth_texture_.createEmpty(width, height);

		if (width <= 1.0f)
		{
			// Relative to absolute
			width  *= (float)GM.window().width();
			height *= (float)GM.window().height();
		}

		r.framebuffers_[handler_].internal_id_= -1;
		r.framebuffers_[handler_].version_ = 0;
		r.framebuffers_[handler_].depth_ = depth;
		r.framebuffers_[handler_].width_  = width;
		r.framebuffers_[handler_].height_ = height;
	}

	void Framebuffer::release()
	{
		if (handler_ >= 0)
		{
			color_texture_.release();
			depth_texture_.release();
			GM.renderer().framebuffers_[handler_].version_ = ConstantValues::DELETED_INTERNAL_ID;
			handler_ = ConstantValues::DELETED_HANDLER;
		}
	}
}