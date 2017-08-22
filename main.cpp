#include "core.h"

Core core;

bool InputHandler::keys[1024];

int main(int argc, char ** argv)
{
    core.begin("Mandelbrot");

    while(!core.shouldClose())
        core.update();

    core.end();

    return 0;
}
