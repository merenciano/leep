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
    
    for (int32_t i = 0; i < 50; ++i)
    {
        for(int32_t j = 0; j < 50; ++i)
        {
            Entity cube = Entity::CreateEntity("Cube_" + std::to_string(i) + "_" + std::to_string(j));
            cube.addComponent<FallSpeed>().speed_ = 1.0f;

            InfiniteFallingLimits& ifl = cube.addComponent<InfiniteFallingLimits>();
            ifl.limit_down_ = -1.0f;
            ifl.limit_up_   =  1.0f;

            Transform& cube_tr = cube.addComponent<Transform>();
            cube_tr.transform_ = glm::scale(cube_tr.transform_, glm::vec3(0.3f, 0.3f, 0.3f));
            cube_tr.transform_ = glm::translate(cube_tr.transform_, glm::vec3(0.3f * i, -0.3f * j, -5.0f));

            Drawable &cube_dw = cube.addComponent<Drawable>();
            cube_dw.geometry_ = cube_geo;
            cube_dw.material_.set_type(MaterialType::MT_PBR);
            cube_dw.material_.set_data(pbr);
            cube_dw.material_.set_texture(trex_texture);
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
    LEEP_INFO("Logic time in microseconds: {0}", logic_timer.duration());
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