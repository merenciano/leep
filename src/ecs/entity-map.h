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
        bool operator==(const EntityIndex &other) const
        {
            return (container == other.container) && (index == other.index);
        }
        struct HashFunction
        {
            size_t operator()(const EntityIndex& ei) const
            {
                size_t container_hash = std::hash<void*>()(ei.container);
                size_t index_hash = std::hash<int32_t>()(ei.index) << 1;
                return container_hash ^ index_hash;
            }
        };
    };

    class EntityMap
    {
    public:
        EntityMap() {}
        ~EntityMap() {}

        void addEntry(std::string name, int32_t id, void *container_ptr)
        {
            EntityIndex eid = { container_ptr, id };
            map_[name] = eid;
            rmap_[eid] = name;
        }

        void swap(int32_t i, int32_t last, void *container_ptr)
        {
            EntityIndex ei = { container_ptr, i };
            EntityIndex elast = { container_ptr, last };
            
            rmap_[ei] = rmap_[elast];
            map_[rmap_[elast]] = ei;
        }

        void removeEntry(std::string name, int32_t id, void *container_ptr)
        {
            EntityIndex ei = { container_ptr, id };
            map_.erase(name);
            rmap_.erase(ei);
        }

        bool exists(std::string name)
        {
            return map_.find(name) != map_.end(); 
        }

        EntityIndex getEntity(std::string name) 
        {
            return map_[name];
        }


    private:
        std::unordered_map<std::string, EntityIndex> map_;
        std::unordered_map<EntityIndex, std::string, EntityIndex::HashFunction> rmap_;
    };
}

#endif // __LEEP_ECS_ENTITY_MAP_H__
