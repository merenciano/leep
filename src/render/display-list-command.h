// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_DISPLAY_LIST_COMMAND_H__
#define __LEEP_DISPLAY_LIST_COMMAND_H__ 1

namespace leep
{
    class DLComm
    {
    public:
        virtual ~DLComm() {}

    protected:
        virtual void executeCommand() const = 0;

        friend class Renderer;
    };
}

#endif // __LEEP_DISPLAY_LIST_COMMAND_H__