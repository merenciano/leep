// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_COMMANDS_CREATE_TEXTURE_H__
#define __LEEP_RENDER_COMMANDS_CREATE_TEXTURE_H__ 1

#include "render/texture.h"
#include "render/display-list-command.h"

namespace leep
{
	enum class TextureFormat
	{
		// Default enum for catch errors
		NONE,

		// Use for image textures (will make the image linear and then will need to be gamma corrected)
		GAMMA,

		// Use for already linear images (normap maps, ao, roughness...)
		LINEAR,

		// Framebuffer attachments
		COLOR_BUFFER,
		DEPTH_BUFFER,
	};

    class CreateTexture : public DisplayListCommand
    {
    public:
        CreateTexture();
        CreateTexture(const CreateTexture &other) = delete;
        CreateTexture(CreateTexture &&other) = delete;
        ~CreateTexture() = default;

        CreateTexture& set_texture(const Texture &texture);
        CreateTexture& set_format(TextureFormat format);
        virtual void executeCommand() const override;

    private:
        Texture texture_;
		TextureFormat format_;
    };
}

#endif // __LEEP_RENDER_COMMANDS_CREATE_TEXTURE_H__