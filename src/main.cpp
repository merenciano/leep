#include "leep.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace leep;

void Init()
{
    Logger::init();
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
            EntityContainer<FallingCubeEntities> &c = GM.memory().ec_falling_;
            Entity<FallingCubeEntities> e = Entity<FallingCubeEntities>::CreateEntity(
                "Cube_" + std::to_string(i) + "_" + std::to_string(j), c);

            Transform &tr = e.getComponent<Transform>();
            tr.transform_ = glm::scale(tr.transform_, glm::vec3(0.3f, 0.3f, 0.3f));
            tr.transform_ = glm::translate(tr.transform_, glm::vec3(1.5f * i, -1.1f * j, -5.0f));

            Drawable &cube_dw = e.getComponent<Drawable>();
            cube_dw.geometry_ = cube_geo;
            cube_dw.material_.set_type(MaterialType::MT_PBR);
            cube_dw.material_.set_data(pbr);
            cube_dw.material_.set_texture(trex_texture);

            e.getComponent<FallSpeed>().speed_ = 0.1f;
            e.getComponent<InfiniteFallingLimits>().limit_down_ = -15.0f;
            e.getComponent<InfiniteFallingLimits>().limit_up_= 15.0f;
        }
    }
}

void Logic()
{
    Chrono logic_timer;
    GM.input().updateInput();
    CameraMovement(1.0f, 1.0f).executeSystem();
    logic_timer.start();
    Fall<FallingCubeEntities>(GM.memory().ec_falling_).executeSystem();
    InfiniteFalling<FallingCubeEntities>(GM.memory().ec_falling_).executeSystem();
    Render<FallingCubeEntities>(GM.memory().ec_falling_).executeSystem();
    logic_timer.end();
    int64_t duration = logic_timer.duration();
#ifdef LEEP_DEBUG
    LEEP_INFO("Logic time in microseconds: {0}", duration);
#else
    printf("Logic time in microseconds: %d\n", duration);
#endif
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
