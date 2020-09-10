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
        window.swap();
        window.pollEvents();
    }

    return 0;
}