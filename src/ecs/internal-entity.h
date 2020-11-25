#ifndef __MOTORET_ECS_INTERNAL_ENTITY_H__
#define __MOTORET_ECS_INTERNAL_ENTITY_H__ 1

#include "ecs/component.h"

#include <stdint.h>
#include <string>
#include <vector>
#include <memory>

namespace motoret
{
    class InternalEntity
    {
    public:
        InternalEntity() = default;
        ~InternalEntity() = default;

        uint64_t componentMask() const
        {
            uint64_t mask = 0;
            for (const std::unique_ptr<Component> &c : components_)
            {
                mask |= (1LL << c.get()->type());
            }
            return mask;
        }
    
        template<typename T>
        T* getComponent()
        {
            for (const std::unique_ptr<Component> &c : components_)
            {
                if (c.get()->type() == T::type)
                {
                    return static_cast<T*>(c.get());
                }
            }
            return nullptr;
        }

        std::string name_;
        std::vector<std::unique_ptr<Component>> components_;
    };
}
#endif // __MOTORET_ECS_INTERNAL_ENTITY_H__