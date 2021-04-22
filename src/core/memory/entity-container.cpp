#include "entity-container.h"
#include "core/common-defs.h"
#include "core/manager.h"
#include "core/memory/memory.h"
#include "core/memory/entity-chunk.h"

namespace leep
{
    EntityContainer::EntityContainer() : type_(EntityType::NONE)
    {

    }

    EntityContainer::EntityContainer(EntityType t) : type_(t)
    {
        Memory &m = GM.memory();
        switch (type_)
        {
            case EntityType::NONE:
                LEEP_ASSERT(false, "Invalid entity type");
                break;
            case EntityType::FALLING_CUBE:
                //blocks_.emplace_back(new FallingCubeEntities());
                blocks_.emplace_back(m.generalAllocT<FallingCubeEntities>(1));
                break;

            case EntityType::RENDERABLE:
                //blocks_.emplace_back(new RenderableEC());
                blocks_.emplace_back(m.generalAllocT<RenderableEC>(1));
                break;
        }
        blocks_.back()->index_ = 0;
    }

    EntityContainer::~EntityContainer()
    {
        Memory &m = GM.memory();
        for (auto c : blocks_)
        {
            m.generalFree(c);
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
        Memory &m = GM.memory();
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
                m.generalFree(blocks_.back());
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