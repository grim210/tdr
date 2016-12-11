#include <fstream>
#include <iostream>
#include <string>

#include <glad/glad.h>
#include "triangle.h"
#include "window.h"

int main(int argc, char* argv[])
{
    std::unique_ptr<Window> window = Window::Initialize(800, 600, false);
    if (window == nullptr) {
        std::cerr << "nullptr returned.  Aborting." << std::endl;
        return -1;
    }
    window->setClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    std::unique_ptr<Triangle> tri = Triangle::Create();

    bool running = true;
    while (running) {
        window->clear();
        tri->draw();
        running = window->swap();
    }

    Triangle::Destroy(tri.get());
    Window::Destroy(window.get());
    return 0;
}
