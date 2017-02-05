#include <cstring>
#include <iostream>

#include "config.h"
#include "help.h"
#include "meshgenerator.h"

int main(int argc, char* argv[])
{
    std::unique_ptr<MeshGenerator> generator(new MeshGenerator());
#ifdef TDR_DEBUG
    for (int i = 0; i < argc; i++) {
        std::cout << argv[i] << std::endl;
    }
#endif

    std::fstream in;
    std::fstream out;
    std::string result;

    switch (argc) {
    case 0:
    case 1:
        Help::Standard();
        exit(EXIT_SUCCESS);
    case 2:
        if (strncmp("-g", argv[1], strlen("-g")) == 0) {
            generator->dump();
        } else {
            Help::Extended(std::string(argv[1]));
        }
        exit(EXIT_SUCCESS);
    }

    return 0;
}
