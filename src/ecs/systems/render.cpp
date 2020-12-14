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
        PbrSceneData pbr_sd;
        pbr_sd.view_projection = GM.camera().view_projection();

        displayl.addCommand<Clear>()
            .set_clear_buffer(true, true, true)
            .set_clear_color(0.2f, 0.2f, 0.2f, 1.0f);

        displayl.addCommand<UsePbrMaterial>()
            .set_scene_data(pbr_sd);

        for (auto &e : GM.entities_)
        {
            if (e.hasComponents(mask))
            {
                Transform *tr = e.getComponent<Transform>();
                Drawable *dw = e.getComponent<Drawable>();

                dw->material_.set_world(tr->transform_);

                displayl.addCommand<Draw>()
                    .set_geometry(dw->geometry_)
                    .set_material(dw->material_);
            }
        }

        displayl.submit();
    }
}