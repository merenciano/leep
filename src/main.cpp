#include <stdio.h>

#include "motoret.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace motoret;

struct DrawableObject
{
    Geometry geometry;
    Material material;
} cube;

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
    //pcd.world = glm::mat4(1.0f);
    pcd.world = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
    pcd.world = glm::translate(pcd.world, glm::vec3(0.0f, 0.0f, -5.0f));
    pcd.world = glm::rotate(pcd.world, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    cube.geometry.createCube();
    cube.material.set_type(MaterialType::MT_PLAIN_COLOR);
    cube.material.set_data(pcd);
}

void Logic()
{
    DisplayList test_dl;
    PlainColorSceneData plain_color_sd;
    plain_color_sd.view_projection = glm::perspective(glm::radians(70.0f), 1280.0f / 720.0f, 0.1f, 50.0f);

    test_dl.addCommand<Clear>()
        .set_clear_buffer(true, true, true)
        .set_clear_color(0.2f, 0.2f, 0.2f, 1.0f);

    test_dl.addCommand<UsePlainColorMaterial>()
        .set_scene_data(plain_color_sd);

    test_dl.addCommand<Draw>()
        .set_geometry(cube.geometry)
        .set_material(cube.material);

    test_dl.submit();
}

void Render()
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
        Render();

        window.swap();
        window.pollEvents();
    }

    return 0;
}