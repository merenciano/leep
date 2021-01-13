#include "entity-map.h"

namespace leep
{
    EntityMap::EntityMap()
    {

    }

    EntityMap::~EntityMap()
    {

    }

    void EntityMap::addEntry(std::string name, int32_t id, EntityType t)
    {
        EntityIndex eid = { t, id };
        map_[name] = eid;
        rmap_[eid] = name;
    }

    void EntityMap::swap(int32_t i, int32_t last, EntityType t)
    {
        EntityIndex ei = { t, i };
        EntityIndex elast = { t, last };
        
        rmap_[ei] = rmap_[elast];
        map_[rmap_[elast]] = ei;
    }

    void EntityMap::removeEntry(std::string name, int32_t id, EntityType t)
    {
        EntityIndex ei = { t, id };
        map_.erase(name);
        rmap_.erase(ei);
    }

    bool EntityMap::exists(std::string name)
    {
        return map_.find(name) != map_.end(); 
    }

    EntityIndex EntityMap::getEntity(std::string name) 
    {
        return map_[name];
    }

    bool EntityIndex::operator==(const EntityIndex &other) const
    {
        return (type == other.type) && (index == other.index);
    }

    size_t EntityIndex::HashFunction::operator()(const EntityIndex& ei) const
    {
        size_t container_hash = std::hash<EntityType>()(ei.type);
        size_t index_hash = std::hash<int32_t>()(ei.index) << 1;
        return container_hash ^ index_hash;
    }
}