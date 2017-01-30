#include <iostream>

#include "config.h"

int main(int argc, char* argv[])
{
    std::cout << "tdrmeshtool v" << TDRMESHTOOL_VERSION_MAJOR << ".";
    std::cout << TDRMESHTOOL_VERSION_MINOR << ".";
    std::cout << TDRMESHTOOL_VERSION_PATCH << std::endl;

    return 0;
}
