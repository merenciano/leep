#include "entity.h"

#include "core/manager.h"
#include "core/memory/memory.h"
#include "core/scene.h"
#include "ecs/components/ltransform.h"

namespace leep
{
    Entity Entity::CreateEntity(String name, EntityType t)
    {
        Memory &m = GM.memory();
        if (GM.scene().entity_map_.exists(name))
        {
            LEEP_CORE_ERROR("CreateEntity: An entity with that name already exists.");
            return Entity(-1, EntityType::NONE);
        }
        EntityContainer &cont = GM.scene().container(t);
        if (cont.blocks_.back()->last_ == kEntitiesPerChunk)
        {
            int32_t idx = cont.blocks_.back()->index_;
            switch (t)
            {
            case EntityType::RENDERABLE:
                cont.blocks_.emplace_back(m.generalAllocT<RenderableEC>(1));
                break;

            case EntityType::FALLING_CUBE:
                cont.blocks_.emplace_back(m.generalAllocT<FallingCubeEntities>(1));
                break;

            default: LEEP_ASSERT(false, "Default case CreateEntity");
            }
            cont.blocks_.back()->index_ = idx + 1;
        }
        int32_t entity_id = cont.blocks_.back()->index_ * kEntitiesPerChunk + cont.blocks_.back()->last_;
        cont.blocks_.back()->last_++;
        GM.scene().entity_map_.addEntry(name, entity_id, t);
        return Entity(entity_id, t);
    }

    void Entity::RemoveEntity(String name)
    {
        EntityMap &emap = GM.scene().entity_map_;
        if (!emap.exists(name))
        {
            // Entity already removed or never created
            // in any case the job is done
            return;
        }
        EntityContainer &cont = GM.scene().container(emap.getEntity(name).type);
        int32_t index = emap.getEntity(name).index;
        int32_t chunk_id = ChunkI(index);
        int32_t entity_id = EntityI(index);

        // Remove the hierarchy of this entity if it had a node.
        LTransform *ltr = GetEntity(name).componentPtr<LTransform>();
        if (ltr)
        {
            GM.scene().scene_graph_.removeNode(ltr);
        }
        
        // Copy the last entity to the place of the removed one
        int32_t last_id = ((int32_t)cont.blocks_.size()-1) * kEntitiesPerChunk +
            (cont.blocks_.back()->last_ - 1);
        if (last_id != index)
        {
            switch (cont.type())
            {
                case EntityType::NONE:
                    LEEP_ASSERT(false, "RemoveEntity(): Invalid entity type");
                    break;

                case EntityType::FALLING_CUBE:
                    static_cast<FallingCubeEntities*>(cont.blocks_.back())
                        ->relocateLast(cont.blocks_[chunk_id], entity_id);
                    break;

                case EntityType::RENDERABLE:
                    static_cast<RenderableEC*>(cont.blocks_.back())
                        ->relocateLast(cont.blocks_[chunk_id], entity_id);

            }
            emap.swap(index, last_id, emap.getEntity(name).type);
        }
        cont.removeLastEntity();
        emap.removeEntry(name, last_id, emap.getEntity(name).type);
    }

    Entity Entity::GetEntity(String name)
    {
        EntityMap &emap = GM.scene().entity_map_;
        LEEP_ASSERT(emap.exists(name), "There isn't any entity with that name");
        return Entity(emap.getEntity(name).index, emap.getEntity(name).type);
    }

    String Entity::name() const
    {
        return GM.scene().entity_map_.getEntityName(index_, type_);
    }

    bool Entity::isValid() const
    {
        return index_ >= 0 ? true : false;
    }

    Entity& Entity::operator=(const Entity& e)
    {
        index_ = e.index_;
        type_ = e.type_;
        return *this;
    }
}