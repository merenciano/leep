// Lucas Merenciano Martinez  <lucasmermar@gmail.com>

#ifndef __LEEP_ECS_ENTITY_MAP_H__
#define __LEEP_ECS_ENTITY_MAP_H__

#include "core/common-defs.h"

#include <stdint.h>
#include <unordered_map>
#include <string>

namespace leep
{
    // Using this struct instead of Entity directly because I dont want to
    // do to the class Entity all the necesary things to make it work
    // in the unordered_map, like having a default constructor
    struct EntityIndex
    {
        EntityType type;
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

        void addEntry(std::string name, int32_t id, EntityType t);
        void swap(int32_t i, int32_t last, EntityType t);
        void removeEntry(std::string name, int32_t id, EntityType t);
        bool exists(std::string name);
        EntityIndex getEntity(std::string name);
        std::string getEntityName(int32_t i, EntityType t);

    private:
        std::unordered_map<std::string, EntityIndex> map_;
        std::unordered_map<EntityIndex, std::string, EntityIndex::HashFunction> rmap_;
    };
}

#endif // __LEEP_ECS_ENTITY_MAP_H__
