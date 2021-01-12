// Lucas Merenciano Martinez  <lucasmermar@gmail.com>

#ifndef __LEEP_CORE_MEMORY_ENTITY_CONTAINER_H__
#define __LEEP_CORE_MEMORY_ENTITY_CONTAINER_H__ 1

#include "core/memory/entity-chunk.h"

#include <deque>

namespace leep
{
    class EntityContainer
    {
    public:
        EntityContainer(EntityType t);
        ~EntityContainer();

        bool isEmpty() const;
        void removeLastEntity();
        EntityType type() const;

        std::deque<EntityChunk*> chunks_;
    private:
        const EntityType type_;
    };
}
#endif // __LEEP_CORE_MEMORY_ENTITY_CONTAINER_H__
