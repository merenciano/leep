#include "entity.h"

namespace leep
{
    Entity Entity::CreateEntity(std::string name, EntityContainer &cont)
    {
        LEEP_CORE_ASSERT(!s_map_.exists(name), "An entity with that name already exists.");
        if (cont.chunks_.back()->last_ == kEntitiesPerChunk)
        {
            int32_t idx = cont.chunks_.back()->index_;
            switch (cont.type())
            {
                case EntityType::RENDERABLE:
                    cont.chunks_.emplace_back(new RenderableEC());
                    break;

                case EntityType::FALLING_CUBE:
                    cont.chunks_.emplace_back(new FallingCubeEntities());
                    break;

                default: LEEP_ASSERT(false, "Default case CreateEntity");
            }
            cont.chunks_.back()->index_ = idx + 1;
        }
        int32_t entity_id = cont.chunks_.back()->index_ * kEntitiesPerChunk + cont.chunks_.back()->last_;
        cont.chunks_.back()->last_++;
        s_map_.addEntry(name, entity_id, &cont);
        return Entity(entity_id, cont);
    }

    void Entity::RemoveEntity(std::string name)
    {
        if (!s_map_.exists(name))
        {
            // Entity already removed or never created
            // in any case the job is done
            return;
        }
        EntityContainer &cont = *(s_map_.getEntity(name).container);
        int32_t index = s_map_.getEntity(name).index;
        int32_t chunk_id = ChunkI(index);
        int32_t entity_id = EntityI(index);
        
        // Copy the last entity to the place of the removed one
        int32_t last_id = (cont.chunks_.size()-1) * kEntitiesPerChunk + (cont.chunks_.back()->last_ - 1);
        if (last_id != index)
        {
            switch (cont.type())
            {
                case EntityType::NONE:
                    LEEP_ASSERT(false, "RemoveEntity(): Invalid entity type");
                    break;

                case EntityType::FALLING_CUBE:
                    static_cast<FallingCubeEntities*>(cont.chunks_.back())
                        ->relocateLast(cont.chunks_[chunk_id], entity_id);
                    break;

                case EntityType::RENDERABLE:
                    static_cast<RenderableEC*>(cont.chunks_.back())
                        ->relocateLast(cont.chunks_[chunk_id], entity_id);

            }
            s_map_.swap(index, last_id, &cont);
        }
        cont.removeLastEntity();
        s_map_.removeEntry(name, last_id, &cont);
    }

    Entity Entity::GetEntity(std::string name)
    {
        LEEP_ASSERT(s_map_.exists(name), "There isn't any entity with that name");
        return Entity(s_map_.getEntity(name).index, *(s_map_.getEntity(name).container));
    }

    bool Entity::isValid() const
    {
        return index_ >= 0 ? true : false;
    }
}