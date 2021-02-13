#include "framebuffer.h"
#include "core/common-defs.h"
#include "core/manager.h"
#include "core/window.h"
#include "render/renderer.h"

namespace leep
{
	Framebuffer::Framebuffer()
	{
		handler_ = ConstantValues::UNINITIALIZED_HANDLER;
	}

	Framebuffer::~Framebuffer()
	{

	}
	
	void Framebuffer::create(float width, float height, bool color, bool depth)
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

		r.framebuffers_[handler_].color_texture_.createEmpty(width, height);
		r.framebuffers_[handler_].depth_texture_.createEmpty(width, height);

		if (width <= 1.0f)
		{
			// Relative to absolute
			width  *= (float)GM.window().width();
			height *= (float)GM.window().height();
		}

		r.framebuffers_[handler_].internal_id_= -1;
		r.framebuffers_[handler_].cpu_version_ = 1;
		r.framebuffers_[handler_].gpu_version_ = 0;
		r.framebuffers_[handler_].color_ = color;
		r.framebuffers_[handler_].depth_ = depth;
		r.framebuffers_[handler_].width_  = width;
		r.framebuffers_[handler_].height_ = height;
	}

	void Framebuffer::release()
	{
		if (handler_ >= 0)
		{
			InternalFramebuffer ifb = GM.renderer().framebuffers_[handler_];
			ifb.color_texture_.release();
			ifb.depth_texture_.release();
			ifb.cpu_version_ = ConstantValues::MARKED_FOR_DELETE;
			handler_ = ConstantValues::DELETED_HANDLER;
		}
	}

	void Framebuffer::set_color_texture(Texture color)
	{
		LEEP_CORE_ASSERT(handler_ >= 0, "Framebuffer::set_color_texture: Invalid framebuffer");
		InternalFramebuffer ifb = GM.renderer().framebuffers_[handler_];
		ifb.color_texture_ = color;
		ifb.cpu_version_++;
	}

	void Framebuffer::set_depth_texture(Texture depth)
	{
		LEEP_CORE_ASSERT(handler_ >= 0, "Framebuffer::set_depth_texture: Invalid framebuffer");
		InternalFramebuffer ifb = GM.renderer().framebuffers_[handler_];
		ifb.depth_texture_ = depth;
		ifb.cpu_version_++;
	}

	Texture Framebuffer::color() const
	{
		return GM.renderer().framebuffers_[handler_].color_texture_;
	}

	Texture Framebuffer::depth() const
	{
		return GM.renderer().framebuffers_[handler_].depth_texture_;
	}

	int32_t Framebuffer::id() const
	{
		return handler_;
	}
}