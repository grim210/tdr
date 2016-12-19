#include <iostream>
#include <mainutil.h>
#include <rendererutil.h>

int main(int argc, char* argv[])
{
    std::cout << "Main: " << tdrmain_version() << std::endl;
    std::cout << "Renderer: " << tdrrenderer_version() << std::endl;

    std::cout << load_text_file("./Makefile") << std::endl;
    return 0;
}
