#include <stdio.h>

#include "motoret.h"

using namespace motoret;

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