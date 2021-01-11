// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_ECS_COMPONENT_H__
#define __LEEP_ECS_COMPONENT_H__ 1

namespace leep
{
    enum CompType
    {
        COMP_INVALID = 0,
        COMP_LTRANSFORM,
        COMP_DRAWABLE,
        COMP_FALL_SPEED,
        COMP_INFINITE_FALLING_LIMITS,
        COMP_GTRANSFORM,


        COMP_MAX_VALUE = 63
    };

    class Component
    {
    public:
        explicit Component(CompType t) : type_(t) {}
        virtual ~Component() {};
        CompType type() const { return type_; }

    protected:
        const CompType type_;
    };
}

#endif // __LEEP_ECS_COMPONENT_H__