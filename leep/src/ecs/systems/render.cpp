#include "render.h"

#include "core/manager.h"
#include "core/memory/entity-container.h"
#include "ecs/components/gtransform.h"
#include "ecs/components/drawable.h"
#include "render/display-list.h"
#include "render/camera.h"
#include "render/commands/clear.h"
#include "render/commands/draw.h"
#include "render/commands/use-framebuffer.h"
#include "render/Crendercommands.h"
#include "render/Crenderer.h"

namespace leep
{
    void Render::executeSystem() const
    {
#ifdef LEEP_DEBUG
        uint64_t mask = ((1 << COMP_DRAWABLE) | (1 << COMP_GTRANSFORM));
        LEEP_ASSERT((container_.mask() & mask) == mask, "This type of entity is not valid for this system");
#endif
        //DisplayList displayl;
	THE_RenderCommand *first = NULL;
	THE_RenderCommand *prev = NULL;

	// TODO: En realidad estoy yendo de uno en uno..
	// deberia poder coger el chunk entero y renderizarlo en un
	// solo comando.
        for (auto &chunk : container_.blocks_)
        {
            GTransform *tr_array = chunk->template component<GTransform>();
            Drawable *dw_array = chunk->template component<Drawable>();

            for(int32_t i = 0; i < chunk->last_; ++i)
            {
		//THE_RenderCommand *comm = (THE_RenderCommand*)malloc(sizeof(*comm));
		THE_RenderCommand *comm = THE_AllocateCommand();
		comm->next = NULL;
		if (!first) {
			first = comm;
		}

		const glm::mat4 &tr = tr_array[i].gtr_;
		dw_array[i].material_.set_model((float*)&tr);
		// TODO: Quitar esta mierda
		//comm->data.draw.mat = *(new (&comm->data.draw.mat) Material());
		comm->data.draw.mat = &(dw_array[i].material_);
		comm->data.draw.geometry = dw_array[i].geometry_;
		comm->data.draw.inst_count = 1;
		comm->execute = THE_DrawExecute;
		if (prev) {
			prev->next = comm;
		}

		prev = comm;
            }
        }
	THE_AddCommands(first);
    }
}
