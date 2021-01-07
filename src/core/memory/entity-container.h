// Lucas Merenciano Martinez  <lucasmermar@gmail.com>

#ifndef __LEEP_CORE_MEMORY_ENTITY_CONTAINER_H__
#define __LEEP_CORE_MEMORY_ENTITY_CONTAINER_H__ 1

#include "core/memory/entity-chunk.h"

#include <string>
#include <deque>
#include <unordered_map>

namespace leep
{
    template <typename T>
    struct EntityContainer
    {
        EntityContainer();
        ~EntityContainer();

        bool isEmpty() const;
        void removeLastEntity();

        std::deque<T> chunks_;
        //std::unordered_map<std::string, uint32_t> dictionary_;
        //std::unordered_map<uint32_t, std::string> reverse_dictionary_;
    };


    // Definition
    template<typename T>
    EntityContainer<T>::EntityContainer()
    {
        chunks_.emplace_back(); chunks_.back().index_ = 0;
    }

    template<typename T>
    EntityContainer<T>::~EntityContainer()
    {
#ifdef LEEP_DEBUG
        chunks_.clear();
        //dictionary_.clear();
        //reverse_dictionary_.clear();
#endif
    }

    template<typename T>
    bool EntityContainer<T>::isEmpty() const
    {
        return chunks_.size() == 1 && chunks_.back().last_ == 0;
    }

    template<typename T>
    void EntityContainer<T>::removeLastEntity()
    {
        if (isEmpty())
        {
            return;
        }

        chunks_.back().last_--;
        if (chunks_.back().last_ <= 0)
        {
            if (chunks_.size() == 1)
            {
                chunks_.back().last_ = 0;
            }
            else
            {
                chunks_.pop_back();
            }
        }
    }
}
#endif // __LEEP_CORE_MEMORY_ENTITY_CONTAINER_H__
