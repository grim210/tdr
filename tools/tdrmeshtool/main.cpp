#include <cstring>
#include <iostream>

#include "config.h"
#include "help.h"

int main(int argc, char* argv[])
{
#ifdef TDR_DEBUG
    for (int i = 0; i < argc; i++) {
        std::cout << argv[i] << std::endl;
    }
#endif

    switch (argc) {
    case 0:
    case 1:
        Help::Standard();
        exit(EXIT_SUCCESS);
    case 2:
        Help::Extended(std::string(argv[1]));
        exit(EXIT_SUCCESS);
    }

    return 0;
}
