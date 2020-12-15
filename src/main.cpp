#include "leep.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace leep;

void Init()
{
    GM.init();
    DisplayList init_dl;
    Texture trex_texture;

    trex_texture.create("../assets/tex/trex.jpg");

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

    Entity cube = Entity::CreateEntity("Cube");
    cube.addComponent<FallSpeed>();
    FallSpeed& fall_speed = cube.getComponent<FallSpeed>();
    fall_speed.speed_ = 1.0f;
    cube.addComponent<InfiniteFallingLimits>();
    cube.getComponent<InfiniteFallingLimits>().limit_down_ = -1.0f;
    cube.getComponent<InfiniteFallingLimits>().limit_up_ = 1.0f;
    cube.addComponent<Transform>();
    cube.addComponent<Drawable>();
    Transform* cube_tr = &(cube.getComponent<Transform>());
    cube_tr->transform_ = glm::scale(cube_tr->transform_, glm::vec3(0.03f, 0.03f, 0.03f));
    cube_tr->transform_ = glm::translate(cube_tr->transform_, glm::vec3(0.0f, -3.0f, -5.0f));
    //cube_tr->transform_ = glm::rotate(cube_tr->transform_, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    Drawable* cube_dw = &(cube.getComponent<Drawable>());
    //cube_dw->geometry_.createCube();
    cube_dw->geometry_.loadObj("../assets/obj/trex.obj");
    cube_dw->material_.set_type(MaterialType::MT_PBR);
    cube_dw->material_.set_data(pbr);
    cube_dw->material_.set_texture(trex_texture);
}

void Logic()
{
    Chrono logic_timer;
    logic_timer.start();
    GM.input().updateInput();
    CameraMovement(1.0f, 1.0f).executeSystem();
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