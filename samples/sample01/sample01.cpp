#include <iostream>
#include <tdrmain.h>
#include <tdrrenderer.h>

int main(int argc, char* argv[])
{
    std::cout << "Main:     " << tdrmain_version() << std::endl;
    std::cout << "Renderer: " << tdrrenderer_version() << std::endl;

    return 0;
}
