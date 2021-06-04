// Lucas Merenciano Martinez  <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_INTERNAL_RESOURCES_H__
#define __LEEP_RENDER_INTERNAL_RESOURCES_H__

#include "core/common-defs.h"
#include "render/material.h"
#include "render/texture.h"

namespace leep
{
    class InternalMaterial
    {
    public:
        virtual ~InternalMaterial() {}

        virtual void init() = 0;
        virtual void useMaterialData(const Material &material) const = 0;
        virtual uint32_t internal_id() const = 0;

    protected:
        uint32_t internal_id_;
    };

    struct InternalTexture
    {
        char path_[64];
        void *pix_;
        int32_t internal_id_;
        int32_t cpu_version_;
        int32_t gpu_version_;
        uint32_t texture_unit_;
        uint32_t width_;
        uint32_t height_;
        TexType type_;
    };

	struct InternalFramebuffer
	{
		int32_t internal_id_;
		int32_t cpu_version_;
        int32_t gpu_version_;
		float width_;
		float height_;
        Texture color_texture_;
        Texture depth_texture_;
		bool color_;
		bool depth_;
	};

    struct InternalBuffer
    {
        InternalBuffer()
        {
            internal_id_ = CommonDefs::UNINIT_INTERNAL_ID;
            cpu_version_ = 0;
            gpu_version_ = 0;
            count_ = 0;
            data_.vertices_ = nullptr;
        }

        InternalBuffer(const InternalBuffer& other) = delete;
		InternalBuffer(InternalBuffer&& other) = default;

        ~InternalBuffer()
        {
            LEEP_CORE_INFO("InternalBuffer data freed");
        }
        // The data will be unsigned or float depending on the type of the buffer
        union BufferData
        {
            float *vertices_;
            uint32_t *indices_;
        } data_;
        int32_t count_;
        uint32_t internal_id_;
        int32_t cpu_version_;
        int32_t gpu_version_;
    };
}

#endif // __LEEP_RENDER_INTERNAL_RESOURCES_H__