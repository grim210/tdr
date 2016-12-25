#include <iostream>
#include <tdrmain.h>
#include <tdrrenderer.h>

int main(int argc, char* argv[])
{
    char buff[16];

    tdrmain_version(buff, 16);
    std::cout << "Main:     " << buff << std::endl;

    tdrrenderer_version(buff, 16);
    std::cout << "Renderer: " << buff << std::endl;

    return 0;
}
