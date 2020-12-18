// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_ECS_SYSTEMS_RENDER_H__
#define __LEEP_ECS_SYSTEMS_RENDER_H__ 1

#include "ecs/system.h"
#include "render/commands/use-pbr-material.h"
#include "render/commands/clear.h"
#include "render/commands/draw.h"

#include "glm/gtc/matrix_transform.hpp"

namespace leep
{
    template<typename T>
    class Render : public System
    {
    public:
        Render() = delete;
        Render(EntityContainer<T> &container) : container_(container) {}
        EntityContainer<T> &container_;
        virtual void executeSystem() const override
        {
            DisplayList displayl;
            uint64_t mask = ((1 << COMP_DRAWABLE) | (1 << COMP_TRANSFORM));
            LEEP_ASSERT((T::mask & mask) == mask, "This type of entity is not valid for this system");
            PbrSceneData pbr_sd;
            pbr_sd.view_projection = GM.camera().view_projection();

            displayl.addCommand<Clear>()
                .set_clear_buffer(true, true, true)
                .set_clear_color(0.2f, 0.2f, 0.2f, 1.0f);

            displayl.addCommand<UsePbrMaterial>()
                .set_scene_data(pbr_sd);

            for (auto &chunk : container_.chunks_)
            {
                for(int32_t i = 0; i < chunk.last_; ++i)
                {
                    const glm::mat4 &tr = chunk.transform[i].transform_;
                    Drawable &dw = chunk.drawable[i];

                    dw.material_.set_world(tr);
                    displayl.addCommand<Draw>()
                        .set_geometry(dw.geometry_)
                        .set_material(dw.material_);
                }
            }

            displayl.submit();
        }
    };
}

#endif // __LEEP_ECS_SYSTEMS_RENDER_H__