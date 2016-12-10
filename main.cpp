#include <iostream>
#include "window.h"

int main(int argc, char* argv[])
{
    std::unique_ptr<Window> window = Window::Initialize(800, 600, false);
    if (window == nullptr) {
        std::cerr << "nullptr returned.  Aborting." << std::endl;
        return -1;
    }

    bool running = true;
    while (running) {
        window->clear();
        running = window->swap();
    }

    Window::Destroy(window.get());
    return 0;
}
