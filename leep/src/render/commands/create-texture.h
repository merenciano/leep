// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_COMMANDS_CREATE_TEXTURE_H__
#define __LEEP_RENDER_COMMANDS_CREATE_TEXTURE_H__ 1

#include "render/texture.h"
#include "render/display-list-command.h"

namespace leep
{
    class CreateTexture : public DLComm
    {
    public:
        CreateTexture();
        CreateTexture(const CreateTexture &other) = delete;
        CreateTexture(CreateTexture &&other) = delete;
        ~CreateTexture() = default;

        CreateTexture& set_texture(const Texture &texture);

        // If true, releases pixel data from RAM after
        // being copied to VRAM by the render backend
        CreateTexture& releaseRamData(bool value);
        virtual void executeCommand() const override;

    private:
        Texture texture_;
        bool release_ram_data_;
    };
}

#endif // __LEEP_RENDER_COMMANDS_CREATE_TEXTURE_H__