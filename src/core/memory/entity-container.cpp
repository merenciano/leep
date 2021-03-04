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
                blocks_.emplace_back(new FallingCubeEntities());
                break;

            case EntityType::RENDERABLE:
                blocks_.emplace_back(new RenderableEC());
                break;
        }
        blocks_.back()->index_ = 0;
    }

    EntityContainer::~EntityContainer()
    {
        for (auto c : blocks_)
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
        return blocks_.size() == 1 && blocks_.back()->last_ == 0;
    }

    void EntityContainer::removeLastEntity()
    {
        if (isEmpty())
        {
            return;
        }

        blocks_.back()->last_--;
        if (blocks_.back()->last_ <= 0)
        {
            if (blocks_.size() == 1)
            {
                blocks_.back()->last_ = 0;
            }
            else
            {
                delete blocks_.back();
                blocks_.pop_back();
            }
        }
    }

#ifdef LEEP_DEBUG
    uint64_t EntityContainer::mask() const
    {
        return blocks_.front()->mask();
    }
#endif
}