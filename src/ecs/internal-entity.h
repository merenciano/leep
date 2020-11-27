// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_ECS_INTERNAL_ENTITY_H__
#define __LEEP_ECS_INTERNAL_ENTITY_H__ 1

#include "ecs/component.h"

#include <stdint.h>
#include <string>
#include <vector>
#include <memory>

namespace leep 
{
    struct InternalEntity
    {
        InternalEntity() = default;
        InternalEntity(std::string name) : name_(name) {}
        ~InternalEntity() = default;

        uint64_t componentMask() const
        {
            uint64_t mask = 0;
            for (const std::shared_ptr<Component> &c : components_)
            {
                mask |= (1LL << c.get()->type());
            }
            return mask;
        }

        bool hasComponents(uint64_t mask) const
        {
            return (mask & componentMask()) == mask;
        }

        void swap(InternalEntity *ie)
        {
            std::swap(name_, ie->name_);
            std::swap(components_, ie->components_);
        }
    
        template<typename T>
        T* getComponent()
        {
            for (const std::shared_ptr<Component> &c : components_)
            {
                if (c.get()->type() == T::type)
                {
                    return static_cast<T*>(c.get());
                }
            }
            return nullptr;
        }

        std::string name_;
        std::vector<std::shared_ptr<Component>> components_;
    };
}
#endif // __LEEP_ECS_INTERNAL_ENTITY_H__