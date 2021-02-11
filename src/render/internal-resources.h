// Lucas Merenciano Martinez  <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_INTERNAL_RESOURCES_H__
#define __LEEP_RENDER_INTERNAL_RESOURCES_H__

#include "core/common-defs.h"
#include "render/material.h"
#include <string>

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
        int32_t internal_id_ = -1;
        uint32_t texture_unit_ = 0;
        int32_t version_ = 0;
        uint32_t width_;
        uint32_t height_;
        std::string path_ = "";
		bool linear_;
        bool cube_;
    };

	struct InternalFramebuffer
	{
		int32_t internal_id_ = -1;
		int32_t version_ = 0;
		float width_;
		float height_;
		bool color_;
		bool depth_;
	};

    struct InternalBuffer
    {
        InternalBuffer()
        {
            internal_id_ = ConstantValues::UNINITIALIZED_INTERNAL_ID;
            version_ = 0;
            gpu_version_ = 0;
        }

        InternalBuffer(const InternalBuffer& other) = delete;
		InternalBuffer(InternalBuffer&& other) = default;

        ~InternalBuffer()
        {
            LEEP_CORE_INFO("InternalBuffer data freed");
        }

        uint32_t internal_id_;
        int32_t version_;
        int32_t gpu_version_;

        //  TODO: I don't like having one unused vector in every buffer
        //  but I didn't be able to make it work with unions because
        //  of the exceptions it threw, I whink multiple std::vectors
        //  inside an union does not work well, but I need some research on this
        std::vector<Vertex> vertices_data_;
        std::vector<uint32_t> indices_data_;
    };
}

#endif // __LEEP_RENDER_INTERNAL_RESOURCES_H__