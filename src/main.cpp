#include "leep.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace leep;

void Init()
{
    GM.init();
    DisplayList init_dl;
    Texture trex_texture;
    Geometry cube_geo;

    trex_texture.create("../assets/tex/trex.jpg");
    cube_geo.createCube();

    init_dl.addCommand<RenderOptions>()
        .set_depth(true, true)
        .set_cull_face(CullFace::BACK)
        .set_blend(BlendFunc::ONE, BlendFunc::ZERO);

    init_dl.addCommand<InitMaterial>()
        .set_material(MaterialType::MT_PBR);

    init_dl.submit();

    PbrData pbr;
    pbr.tiling_x_ = 1.0f;
    pbr.tiling_y_ = 1.0f;
    
    for (int32_t i = 0; i < 100; ++i)
    {
        for(int32_t j = 0; j < 100; ++j)
        {
            int32_t entity_index = i * 100 + j;

            Transform& tr = GM.stack_memory_.falling_cube_entities_.transform[entity_index];
            tr.transform_ = glm::scale(tr.transform_, glm::vec3(0.3f, 0.3f, 0.3f));
            tr.transform_ = glm::translate(tr.transform_, glm::vec3(1.1f * i, -1.1f * j, -5.0f));

            Drawable &cube_dw = GM.stack_memory_.falling_cube_entities_.drawable[entity_index];
            cube_dw.geometry_ = cube_geo;
            cube_dw.material_.set_type(MaterialType::MT_PBR);
            cube_dw.material_.set_data(pbr);
            cube_dw.material_.set_texture(trex_texture);

            GM.stack_memory_.falling_cube_entities_.fall_speed[entity_index].speed_ = 0.1f;
            GM.stack_memory_.falling_cube_entities_.infinite_falling_limits[entity_index].limit_down_ = -10.0f;
            GM.stack_memory_.falling_cube_entities_.infinite_falling_limits[entity_index].limit_up_ = 10.0f;
        }
    }
}

void Logic()
{
    Chrono logic_timer;
    GM.input().updateInput();
    CameraMovement(1.0f, 1.0f).executeSystem();
    logic_timer.start();
    Fall().executeSystem();
    InfiniteFalling().executeSystem();
    Render().executeSystem();
    logic_timer.end();
    int64_t duration = logic_timer.duration();
    printf("Logic time in microseconds: %d\n", duration);
    LEEP_INFO("Logic time in microseconds: {0}", duration);
}

void RenderScene()
{
    Manager::instance().renderer().renderFrame();
}

int main()
{
    Init();

    while (!GM.window().windowShouldClose())
    {
        Logic();
        GM.renderer().submitFrame();
        RenderScene();

        GM.window().swap();
        GM.window().pollEvents();
    }

    return 0;
}
