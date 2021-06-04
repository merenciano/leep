// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_ECS_SYSTEM_H__
#define __LEEP_ECS_SYSTEM_H__ 1

namespace leep 
{
    class System
    {
    public:
        virtual void executeSystem() const = 0;
    };
}

#endif // __LEEP_ECS_SYSTEM_H__