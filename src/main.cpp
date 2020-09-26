#include <stdio.h>

#include "motoret.h"

using namespace motoret;

void init()
{
    Manager::instance().renderer().init();
    DisplayList init_dl;

    init_dl.addCommand<InitMaterial>()
        .set_material(MaterialType::PLAIN_COLOR)
        .executeCommand();
    
    init_dl.submit();

}

void Logic()
{
    DisplayList test_dl;

    test_dl.addCommand<Clear>()
        .set_clear_buffer(true, true, true)
        .set_clear_color(1.0f, 0.0f, 0.0f, 1.0f)
        .executeCommand();

    test_dl.addCommand<UsePlainColorMaterial>()
        .executeCommand();

    /*test_dl.addCommand<Draw>()
        .set_geometry(Geometry)
        .set_material(Material)
        .executeCommand();*/

    test_dl.submit();
    
}

void Render()
{

}

int main()
{
    Window window;
    window.createWindow(1280, 720, true);
    Logger::init();
    MOTORET_CORE_INFO("Hello World!");

    while (!window.windowShouldClose())
    {
        Clear clear;
        clear.set_clear_color(1.0f, 0.0f, 0.0f, 1.0f);
        clear.executeCommand();

        window.swap();
        window.pollEvents();
    }

    return 0;
}