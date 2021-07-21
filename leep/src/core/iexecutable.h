// Lucas Merenciano Martinez <lucasmermar@gmail.com>

// This interface can be used by any systems which need to
// store different classes and execute all of them without
// care its type. Ex: Systems, DLCommands or PostProcessing effects

#ifndef __LEEP_CORE_INTERFACE_EXECUTABLE_H__
#define __LEEP_CORE_INTERFACE_EXECUTABLE_H__

namespace leep
{
    class IExecutable
    {
        virtual void execute() = 0;
    };
}

#endif // __LEEP_CORE_INTERFACE_EXECUTABLE_H__
