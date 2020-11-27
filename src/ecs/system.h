#ifndef __MOTORET_ECS_SYSTEM_H__
#define __MOTORET_ECS_SYSTEM_H__ 1

namespace motoret
{
    class System
    {
    public:
        virtual void executeSystem() const = 0;
    };
}

#endif // __MOTORET_ECS_SYSTEM_H__