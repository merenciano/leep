#include "entity.h"

#include "core/manager.h"
#include "core/memory/memory.h"
#include "core/scene-graph.h"
#include "ecs/components/ltransform.h"

namespace leep
{
    Entity Entity::CreateEntity(std::string name, EntityType t)
    {
        //LEEP_CORE_ASSERT(!s_map_.exists(name), "An entity with that name already exists.");
        Memory &m = GM.memory();
        if (s_map_.exists(name))
        {
            LEEP_CORE_ERROR("CreateEntity: An entity with that name already exists.");
            return Entity(-1, EntityType::NONE);
        }
        EntityContainer &cont = GM.memory().container(t);
        if (cont.blocks_.back()->last_ == kEntitiesPerChunk)
        {
            int32_t idx = cont.blocks_.back()->index_;
            switch (t)
            {
            case EntityType::RENDERABLE:
                //cont.blocks_.emplace_back(new RenderableEC());
                cont.blocks_.emplace_back(m.generalAllocT<RenderableEC>(1));
                break;

            case EntityType::FALLING_CUBE:
                //cont.blocks_.emplace_back(new FallingCubeEntities());
                cont.blocks_.emplace_back(m.generalAllocT<FallingCubeEntities>(1));
                break;

            default: LEEP_ASSERT(false, "Default case CreateEntity");
            }
            cont.blocks_.back()->index_ = idx + 1;
        }
        int32_t entity_id = cont.blocks_.back()->index_ * kEntitiesPerChunk + cont.blocks_.back()->last_;
        cont.blocks_.back()->last_++;
        s_map_.addEntry(name, entity_id, t);
        return Entity(entity_id, t);
    }

    void Entity::RemoveEntity(std::string name)
    {
        if (!s_map_.exists(name))
        {
            // Entity already removed or never created
            // in any case the job is done
            return;
        }
        EntityContainer &cont = GM.memory().container(s_map_.getEntity(name).type);
        int32_t index = s_map_.getEntity(name).index;
        int32_t chunk_id = ChunkI(index);
        int32_t entity_id = EntityI(index);

        // Remove the hierarchy of this entity if it had a node.
        LTransform *ltr = GetEntity(name).componentPtr<LTransform>();
        if (ltr)
        {
            GM.scene_graph().removeNode(ltr);
        }
        
        // Copy the last entity to the place of the removed one
        int32_t last_id = ((int32_t)cont.blocks_.size()-1) * kEntitiesPerChunk + (cont.blocks_.back()->last_ - 1);
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
            s_map_.swap(index, last_id, s_map_.getEntity(name).type);
        }
        cont.removeLastEntity();
        s_map_.removeEntry(name, last_id, s_map_.getEntity(name).type);
    }

    Entity Entity::GetEntity(std::string name)
    {
        LEEP_ASSERT(s_map_.exists(name), "There isn't any entity with that name");
        return Entity(s_map_.getEntity(name).index, s_map_.getEntity(name).type);
    }

    std::string Entity::name() const
    {
        return s_map_.getEntityName(index_, type_);
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