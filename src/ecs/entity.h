// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_ECS_ENTITY_H__
#define __LEEP_ECS_ENTITY_H__ 1

#include "core/common-defs.h"
#include "core/manager.h"
#include "core/memory/entity-container.h"

#include <stdint.h>

namespace leep
{
    template<typename T>
    class Entity
    {
    public:
        Entity() = delete;
        Entity(int32_t index, const EntityContainer<T> &container) : index_(index), container_(container) {}
        ~Entity() = default;

        static Entity CreateEntity(std::string name, EntityContainer<T> &cont)
        {
            LEEP_CORE_ASSERT(cont.dictionary_.find(name) == cont.dictionary_.end(), "An entity with that name already exists.");
            if (cont.chunks_.back().last_ == kEntitiesPerChunk)
            {
                int32_t idx = cont.chunks_.back().index_;
                cont.chunks_.emplace_back();
                cont.chunks_.back().index_ = idx + 1;
            }
            int32_t entity_id = cont.chunks_.back().index_ * kEntitiesPerChunk + cont.chunks_.back().last_;
            cont.chunks_.back().last_++;
            cont.dictionary_[name] = entity_id;
            cont.reverse_dictionary_[entity_id] = name;
            return Entity(entity_id, cont);
        }

        static void RemoveEntity(std::string name, EntityContainer<T> &cont)
        {
            if (cont.dictionary_.find(name) == cont.dictionary_.end())
            {
                // Entity already removed or never created
                // in any case the job is done
                return;
            }
            int32_t index = cont.dictionary_[name];
            int32_t chunk_id = ChunkI(index);
            int32_t entity_id = EntityI(index);
            
            // Copy the last entity to the place of the removed one
            int32_t last_id = (cont.chunks_.size()-1) * kEntitiesPerChunk + (cont.chunks_.back().last_ - 1);
            if (last_id != index)
            {
                cont.chunks_.back().relocateLast(&(cont.chunks_[chunk_id]), entity_id);
                cont.reverse_dictionary_[index] = cont.reverse_dictionary_[last_id];
                cont.dictionary_[cont.reverse_dictionary_[last_id]] = index;
            }
            cont.removeLastEntity();
            cont.dictionary_.erase(name);
            cont.reverse_dictionary_.erase(last_id);
        }

        static Entity GetEntity(std::string name, EntityContainer<T> &cont)
        {
            LEEP_ASSERT(cont.dictionary_.find(name) != cont.dictionary_.end(), "There isn't any entity with this name in this container.");
            return Entity(cont.dictionary_[name], cont);
        }

        template<typename C>
        C& getComponent()
        {
            for (int32_t i = 0; i < container_.chunks_.at(ChunkI(index_)).comps_.size(); ++i)
            {
                if (C::type == container_.chunks_.at(ChunkI(index_)).comps_[i][0].type())
                {
                    return static_cast<C*>(container_.chunks_.at(ChunkI(index_)).comps_[i])[EntityI(index_)];
                }
            }
            LEEP_ASSERT(false, "The entity does not have this component");
        }

        bool isValid() const { return index_ >= 0 ? true : false; }

        int32_t index_;
        const EntityContainer<T> &container_;
    };
}

#endif // __LEEP_ECS_ENTITY_H__