#include "update-transform.h"

#include "core/memory/entity-container.h"
#include "ecs/components/ltransform.h"
#include "ecs/components/gtransform.h"

namespace leep
{
    void UpdateTransform::executeSystem() const 
    {
#ifdef LEEP_DEBUG
        uint64_t mask = ((1 << COMP_LTRANSFORM) | (1 << COMP_GTRANSFORM));
        LEEP_ASSERT((container_.mask() & mask) == mask, "This container is not valid for this system.");
#endif
        for (auto &chunk : container_.blocks_)
        {
            LTransform *ltr_array = chunk->template component<LTransform>();
            GTransform *gtr_array = chunk->template component<GTransform>();
            for (int32_t i = 0; i < chunk->last_; i++)
            {
                gtr_array[i].gtr_ = ltr_array[i].transform_;
            }
            // Memcpy copies the type too, and making it static and creating a pure virtual func type() adds vtable to components
            //std::memcpy((void*)gtr_array, ltr_array, kEntitiesPerChunk*sizeof(LTransform));
        }
    }
}
