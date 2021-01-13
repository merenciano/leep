#include "entity-container.h"

namespace leep
{
    EntityContainer::EntityContainer() : type_(EntityType::NONE)
    {

    }

    EntityContainer::EntityContainer(EntityType t) : type_(t)
    {
        switch (type_)
        {
            case EntityType::NONE:
                LEEP_ASSERT(false, "Invalid entity type");
                break;
            case EntityType::FALLING_CUBE:
                chunks_.emplace_back(new FallingCubeEntities());
                break;

            case EntityType::RENDERABLE:
                chunks_.emplace_back(new RenderableEC());
                break;
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

#ifdef LEEP_DEBUG
    uint64_t EntityContainer::mask() const
    {
        return chunks_.front()->mask();
    }
#endif
}