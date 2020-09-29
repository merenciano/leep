#include <stdio.h>

#include "motoret.h"

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
    pcd.g = 0.0f;
    pcd.b = 0.0f;
    pcd.a = 1.0f;
    
    cube.geometry.createCube();
    cube.material.set_type(MaterialType::MT_PLAIN_COLOR);
    cube.material.set_data(pcd);
}

void Logic()
{
    DisplayList test_dl;

    test_dl.addCommand<Clear>()
        .set_clear_buffer(true, true, true)
        .set_clear_color(0.2f, 0.2f, 0.2f, 1.0f);

    test_dl.addCommand<UsePlainColorMaterial>();

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