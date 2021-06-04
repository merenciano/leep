#include "render.h"

#include "core/manager.h"
#include "core/memory/entity-container.h"
#include "ecs/components/gtransform.h"
#include "ecs/components/drawable.h"
#include "render/display-list.h"
#include "render/camera.h"
#include "render/commands/use-pbr-material.h"
#include "render/commands/clear.h"
#include "render/commands/draw.h"
#include "render/commands/use-framebuffer.h"

namespace leep
{
    void Render::executeSystem() const
    {
#ifdef LEEP_DEBUG
        uint64_t mask = ((1 << COMP_DRAWABLE) | (1 << COMP_GTRANSFORM));
        LEEP_ASSERT((container_.mask() & mask) == mask, "This type of entity is not valid for this system");
#endif
        DisplayList displayl;

        for (auto &chunk : container_.blocks_)
        {
            GTransform *tr_array = chunk->template component<GTransform>();
            Drawable *dw_array = chunk->template component<Drawable>();
            for(int32_t i = 0; i < chunk->last_; ++i)
            {
                const glm::mat4 &tr = tr_array[i].gtr_;

                dw_array[i].material_.set_model(tr);
                displayl.addCommand<Draw>()
                    .set_geometry(dw_array[i].geometry_)
                    .set_material(dw_array[i].material_);
            }
        }
        displayl.submit();
    }
}
