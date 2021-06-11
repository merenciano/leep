// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_COMMANDS_CREATE_CUBEMAP_H__
#define __LEEP_RENDER_COMMANDS_CREATE_CUBEMAP_H__ 1

#include "render/texture.h"
#include "render/display-list-command.h"

namespace leep
{
    class CreateCubemap : public DLComm
    {
    public:
        CreateCubemap() = default;
        CreateCubemap(const CreateCubemap &other) = delete;
        CreateCubemap(CreateCubemap &&other) = delete;
        ~CreateCubemap() = default;

        CreateCubemap& set_texture(const Texture &texture);
        virtual void executeCommand() const override;

    private:
        Texture texture_;
    };
}

#endif // __LEEP_RENDER_COMMANDS_CREATE_CUBEMAP_H__