#include <stdio.h>

#include "motoret.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace motoret;

void Init()
{
    Manager::instance().renderer().init();
    DisplayList init_dl;

    init_dl.addCommand<InitMaterial>()
        .set_material(MaterialType::MT_PLAIN_COLOR)
        .executeCommand();
    
    init_dl.submit();

    PlainColorData pcd;
    pcd.r = 1.0f;
    pcd.g = 1.0f;
    pcd.b = 0.0f;
    pcd.a = 1.0f;

    Entity cube = Entity::CreateEntity("Cube");
    cube.addComponent<Transform>();
    cube.addComponent<Drawable>();
    Transform* cube_tr = &(cube.getComponent<Transform>());
    cube_tr->transform_ = glm::scale(cube_tr->transform_, glm::vec3(0.2f, 0.2f, 0.2f));
    cube_tr->transform_ = glm::translate(cube_tr->transform_, glm::vec3(0.0f, 0.0f, -5.0f));
    cube_tr->transform_ = glm::rotate(cube_tr->transform_, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    Drawable* cube_dw = &(cube.getComponent<Drawable>());
    cube_dw->geometry_.createCube();
    cube_dw->material_.set_type(MaterialType::MT_PLAIN_COLOR);
    cube_dw->material_.set_data(pcd);
}

void Logic()
{
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
    MOTORET_CORE_INFO("Hello World!");

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