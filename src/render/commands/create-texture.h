// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_COMMANDS_CREATE_TEXTURE_H__
#define __LEEP_RENDER_COMMANDS_CREATE_TEXTURE_H__ 1

#include "render/texture.h"
#include "render/display-list-command.h"

namespace leep
{
    class CreateTexture : public DisplayListCommand
    {
    public:
        CreateTexture() = default;
        CreateTexture(const CreateTexture &other) = delete;
        CreateTexture(CreateTexture &&other) = delete;
        ~CreateTexture() = default;

        CreateTexture& set_texture(const Texture &texture);
        virtual void executeCommand() const override;

    private:
        Texture texture_;
    };
}

#endif // __LEEP_RENDER_COMMANDS_CREATE_TEXTURE_H__