// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_ECS_COMPONENT_H__
#define __LEEP_ECS_COMPONENT_H__ 1

namespace leep
{
    enum CompType
    {
        COMP_INVALID = 0,
        COMP_TRANSFORM,
        COMP_DRAWABLE,


        COMP_MAX_VALUE = 63
    };

    class Component
    {
    public:
        explicit Component(CompType t) : type_(t) {}
        ~Component() = default;
        CompType type() const { return type_; }

    protected:
        const CompType type_;
    };
}

#endif // __LEEP_ECS_COMPONENT_H__