#include <fstream>
#include <iostream>
#include <string>

#include <glad/glad.h>
#include "cube.h"
#include "window.h"

int main(int argc, char* argv[])
{
    std::unique_ptr<Window> window = Window::Initialize(800, 600, false);
    if (window == nullptr) {
        std::cerr << "nullptr returned.  Aborting." << std::endl;
        return -1;
    }
    window->setClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    std::unique_ptr<Cube> cube = Cube::Create();

    bool running = true;
    while (running) {
        window->clear();
        cube->draw();
        running = window->swap();
    }

    Cube::Destroy(cube.get());
    Window::Destroy(window.get());
    return 0;
}
