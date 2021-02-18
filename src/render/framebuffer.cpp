#include "framebuffer.h"
#include "core/common-defs.h"
#include "core/manager.h"
#include "core/window.h"
#include "render/renderer.h"

namespace leep
{
	Framebuffer::Framebuffer()
	{
		handle_ = CommonDefs::UNINIT_HANDLE;
	}

	Framebuffer::~Framebuffer()
	{

	}
	
	void Framebuffer::create(float width, float height, bool color, bool depth)
	{
		LEEP_ASSERT((width > 1.0f && height > 1.0f) ||
					(width <= 1.0f && height <= 1.0f),
					"CreateFramebuffer: Or absolute or relative, make a decision!");
		LEEP_ASSERT(handle_ == CommonDefs::UNINIT_HANDLE, "This framebuffer is already created");
		Renderer &r = GM.renderer();
		if (!r.aviable_fb_pos_.empty())
		{
			handle_ = r.aviable_fb_pos_.front();
			r.aviable_fb_pos_.pop_front();
		}
		else
		{
			InternalFramebuffer ifb;
			r.framebuffers_.push_back(ifb);
			handle_ = (int32_t)r.framebuffers_.size() - 1;
		}

		r.framebuffers_[handle_].color_texture_.createEmpty(width, height, TexType::RGBA_F16);
		r.framebuffers_[handle_].depth_texture_.createEmpty(width, height, TexType::DEPTH);

		if (width <= 1.0f)
		{
			// Relative to absolute
			width  *= (float)GM.window().width();
			height *= (float)GM.window().height();
		}

		r.framebuffers_[handle_].internal_id_= -1;
		r.framebuffers_[handle_].cpu_version_ = 1;
		r.framebuffers_[handle_].gpu_version_ = 0;
		r.framebuffers_[handle_].color_ = color;
		r.framebuffers_[handle_].depth_ = depth;
		r.framebuffers_[handle_].width_  = width;
		r.framebuffers_[handle_].height_ = height;
	}

	void Framebuffer::release()
	{
		if (handle_ >= 0)
		{
			InternalFramebuffer ifb = GM.renderer().framebuffers_[handle_];
			ifb.color_texture_.release();
			ifb.depth_texture_.release();
			ifb.cpu_version_ = CommonDefs::MARKED_FOR_DELETE;
			handle_ = CommonDefs::DELETED_HANDLE;
		}
	}

	void Framebuffer::set_color_texture(Texture color)
	{
        LEEP_CHECK_RESOURCE((*this));
        LEEP_CHECK_RESOURCE(color);
		InternalFramebuffer ifb = GM.renderer().framebuffers_[handle_];
		ifb.color_texture_ = color;
        ifb.color_ = true;
		ifb.cpu_version_++;
	}

	void Framebuffer::set_depth_texture(Texture depth)
	{
        LEEP_CHECK_RESOURCE((*this));
        LEEP_CHECK_RESOURCE(depth);
		InternalFramebuffer ifb = GM.renderer().framebuffers_[handle_];
		ifb.depth_texture_ = depth;
        ifb.depth_ = true;
		ifb.cpu_version_++;
	}

	Texture Framebuffer::color() const
	{
		return GM.renderer().framebuffers_[handle_].color_texture_;
	}

	Texture Framebuffer::depth() const
	{
		return GM.renderer().framebuffers_[handle_].depth_texture_;
	}

	int32_t Framebuffer::handle() const
	{
		return handle_;
	}
}