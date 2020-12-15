#include "render.h"

#include "core/common-defs.h"
#include "core/manager.h"
#include "ecs/components/drawable.h"
#include "ecs/components/transform.h"
#include "render/display-list.h"
#include "render/commands/clear.h"
#include "render/commands/draw.h"
#include "render/commands/use-pbr-material.h"

#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace leep 
{
    void Render::executeSystem() const
    {
        DisplayList displayl;
        uint64_t mask = ((1 << COMP_DRAWABLE) | (1 << COMP_TRANSFORM));
        memory::FallingCubeEntities &entity_chunk = GM.stack_memory_.falling_cube_entities_;
        PbrSceneData pbr_sd;
        pbr_sd.view_projection = GM.camera().view_projection();

        displayl.addCommand<Clear>()
            .set_clear_buffer(true, true, true)
            .set_clear_color(0.2f, 0.2f, 0.2f, 1.0f);

        displayl.addCommand<UsePbrMaterial>()
            .set_scene_data(pbr_sd);

        if ((mask & entity_chunk.mask) == mask)
        {
            for(int32_t i = 0; i < kEntitiesPerChunk; ++i)
            {
                const glm::mat4 &tr = entity_chunk.transform[i].transform_;
                Drawable &dw = entity_chunk.drawable[i];

                dw.material_.set_world(tr);
                displayl.addCommand<Draw>()
                    .set_geometry(dw.geometry_)
                    .set_material(dw.material_);
            }
        }

        displayl.submit();
    }
}