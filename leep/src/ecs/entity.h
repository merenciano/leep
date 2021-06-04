// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_ECS_ENTITY_H__
#define __LEEP_ECS_ENTITY_H__ 1

#include "core/common-defs.h"
#include "core/manager.h"
#include "core/scene.h"
#include "core/memory/memory.h"
#include "core/memory/entity-container.h"
#include "ecs/entity-map.h"

#include <stdint.h>

namespace leep
{
    class Entity
    {
    public:
        Entity() = delete;
        Entity(int32_t index, EntityType t) : index_(index), type_(t) {}
        Entity(const Entity& e) { index_ = e.index_; type_ = e.type_; }
        ~Entity() = default;

        static Entity CreateEntity(String name, EntityType t);
        static void RemoveEntity(String name);
        static Entity GetEntity(String name);

        template<typename C>
        C& getComponent()
        {
            EntityContainer &c = GM.scene().container(type_);
            for (size_t i = 0; i < c.blocks_.at(ChunkI(index_))->comps_.size(); ++i)
            {
                if (C::s_type == c.blocks_.at(ChunkI(index_))->comps_[i][0].type())
                {
                    return static_cast<C*>(c.blocks_.at(ChunkI(index_))->comps_[i])[EntityI(index_)];
                }
            }
            LEEP_ASSERT(false, "The entity does not have this component");
            // Maybe this function should return a pointer but I think it's better to
            // set the assert here instead of checking the return value each time anyone calls this function.
            // This return exists only to remove the compiler warnings. It's after the assert so the method
            // will never return this in debug. In release it will crash (I hope)
            return *static_cast<C*>(nullptr);
        }

        template<typename C>
        C* componentPtr()
        {
            EntityContainer &c = GM.scene().container(type_);
            for (size_t i = 0; i < c.blocks_.at(ChunkI(index_))->comps_.size(); ++i)
            {
                if (C::s_type == c.blocks_.at(ChunkI(index_))->comps_[i][0].type())
                {
                    return static_cast<C*>(c.blocks_.at(ChunkI(index_))->comps_[i]) + EntityI(index_);
                }
            }
            return nullptr;
        }

        template<typename C>
        bool hasComponent()
        {
            // Maybe checking the component mask in the container of the type_
            // is faster but I dont care... this method is not going to be
            // inside any inner loop.
            return componentPtr<C>();
        }

        String name() const;
        bool isValid() const;
        Entity& operator=(const Entity& e);

    private:
        int32_t index_;
        EntityType type_;
    };
}

#endif // __LEEP_ECS_ENTITY_H__
