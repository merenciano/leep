#include "entity-container.h"

namespace leep
{
    EntityContainer::EntityContainer(EntityType t) : type_(t)
    {
        switch (type_)
        {
            case EntityType::FALLING_CUBE:
                chunks_.push_back(new FallingCubeEntities());
        }
        chunks_.back()->index_ = 0;
    }

    EntityContainer::~EntityContainer()
    {
        for (auto c : chunks_)
        {
            delete c;
        }
    }

    EntityType EntityContainer::type() const
    {
        return type_;
    }

    bool EntityContainer::isEmpty() const
    {
        return chunks_.size() == 1 && chunks_.back()->last_ == 0;
    }

    void EntityContainer::removeLastEntity()
    {
        if (isEmpty())
        {
            return;
        }

        chunks_.back()->last_--;
        if (chunks_.back()->last_ <= 0)
        {
            if (chunks_.size() == 1)
            {
                chunks_.back()->last_ = 0;
            }
            else
            {
                delete chunks_.back();
                chunks_.pop_back();
            }
        }
    }
}