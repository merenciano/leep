// Lucas Merenciano Martinez  <lucasmermar@gmail.com>

#ifndef __LEEP_ECS_ENTITY_MAP_H__
#define __LEEP_ECS_ENTITY_MAP_H__

#include <stdint.h>
#include <unordered_map>
#include <string>

namespace leep
{
    struct EntityIndex
    {
        void *container;
        int32_t index;
        bool operator==(const EntityIndex &other) const;
        struct HashFunction
        {
            size_t operator()(const EntityIndex& ei) const;
        };
    };

    class EntityMap
    {
    public:
        EntityMap();
        ~EntityMap();

        void addEntry(std::string name, int32_t id, void *container_ptr);
        void swap(int32_t i, int32_t last, void *container_ptr);
        void removeEntry(std::string name, int32_t id, void *container_ptr);
        bool exists(std::string name);
        EntityIndex getEntity(std::string name);

    private:
        std::unordered_map<std::string, EntityIndex> map_;
        std::unordered_map<EntityIndex, std::string, EntityIndex::HashFunction> rmap_;
    };
}

#endif // __LEEP_ECS_ENTITY_MAP_H__
