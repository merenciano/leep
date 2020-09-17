#ifndef __MOTORET_DISPLAY_LIST_COMMAND_H__
#define __MOTORET_DISPLAY_LIST_COMMAND_H__ 1

namespace motoret
{
    class DisplayListCommand
    {
    public:
        virtual ~DisplayListCommand() {}
        virtual void executeCommand() const = 0;
    };
}

#endif // __MOTORET_DISPLAY_LIST_COMMAND_H__