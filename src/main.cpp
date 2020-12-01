#include "leep.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace leep;

void Init()
{
    Manager::instance().renderer().init();
    DisplayList init_dl;
    Texture trex_texture;

    trex_texture.create("../assets/tex/trex.jpg");

    init_dl.addCommand<InitMaterial>()
        .set_material(MaterialType::MT_PBR);

    init_dl.submit();

    PbrData pbr;
    pbr.tiling_x_ = 1.0f;
    pbr.tiling_y_ = 1.0f;

    Entity cube = Entity::CreateEntity("Cube");
    cube.addComponent<Transform>();
    cube.addComponent<Drawable>();
    Transform* cube_tr = &(cube.getComponent<Transform>());
    cube_tr->transform_ = glm::scale(cube_tr->transform_, glm::vec3(0.02f, 0.02f, 0.02f));
    cube_tr->transform_ = glm::translate(cube_tr->transform_, glm::vec3(0.0f, -3.0f, -5.0f));
    cube_tr->transform_ = glm::rotate(cube_tr->transform_, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    Drawable* cube_dw = &(cube.getComponent<Drawable>());
    cube_dw->geometry_.loadObj("../assets/obj/trex.obj");
    cube_dw->material_.set_type(MaterialType::MT_PBR);
    cube_dw->material_.set_data(pbr);
    cube_dw->material_.set_texture(trex_texture);
}

void Logic()
{
    GM.input().updateInput();
    CameraMovement(1.0f, 1.0f).executeSystem();
    Render().executeSystem();
}

void RenderScene()
{
    Manager::instance().renderer().renderFrame();
}

int main()
{
    Window window;
    window.createWindow(1280, 720, true);
    Logger::init();
    LEEP_CORE_INFO("Hello World!");

    Init();

    while (!window.windowShouldClose())
    {
        Logic();
        Manager::instance().renderer().submitFrame();
        RenderScene();

        window.swap();
        window.pollEvents();
    }

    return 0;
}