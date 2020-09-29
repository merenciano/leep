#ifndef __MOTORET_DISPLAY_LIST_COMMAND_H__
#define __MOTORET_DISPLAY_LIST_COMMAND_H__ 1

namespace motoret
{
    class DisplayListCommand
    {
    public:
        virtual ~DisplayListCommand() {}

    protected:
        virtual void executeCommand() const = 0;

        friend class Renderer;
    };
}

#endif // __MOTORET_DISPLAY_LIST_COMMAND_H__