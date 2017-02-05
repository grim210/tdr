#include "help.h"

void Help::Standard(void)
{
    std::cout << "tdrmeshtool v";
    std::cout << TDRMESHTOOL_VERSION_STRING << std::endl;
    std::cout << "\t-c <in> <out>   Convert to TDR mesh data." << std::endl;
    std::cout << "\t-g <file>       Generate TDR mesh data." << std::endl;
    std::cout << "\t-h              Print this help message." << std::endl;
    std::cout << "\t-v <file>       Validate TDR mesh data." << std::endl;
}

void Help::Extended(std::string option)
{
    if (option.empty()) {
        Help::Standard();
        return;
    }

    char flag = option[1];

    switch (flag) {
    case 'h':
        Help::Standard();
        exit(EXIT_FAILURE);
        break;
    case 'c':
        Help::Convert();
        break;
    case 'g':
        Help::Generate();
        break;
    case 'v':
        Help::Validate();
        break;
    }
}

void Help::Convert(void)
{
    Help::Standard();
    std::cout << std::endl << "Conversion:" << std::endl;
    std::cout << "The conversion options takes a Collada file and converts ";
    std::cout << "it to " << std::endl << "the TDRMesh XML format.  From ";
    std::cout << "there, you'll be able to then view or " << std::endl;
    std::cout << "validate the mesh. " << std::endl;
}

void Help::Generate(void)
{
    Help::Standard();
    std::cout << std::endl << "Generate:" << std::endl;
    std::cout << "This option allows you to generate some dummy datat that ";
    std::cout << "is both valid" << std::endl << "and renderable.  Useful ";
    std::cout << "for demonstrating proper schema and drawing" << std::endl;
    std::cout << "pretty stuff on screen." << std::endl;
}

void Help::Validate(void)
{
    Help::Standard();
    std::cout << std::endl << "Validate:" << std::endl;
    std::cout << "Provide the file name that you would like to validate ";
    std::cout << "and also ensure" << std::endl << "that all data pointed ";
    std::cout << "to by the mesh is present in the working as well.";
    std::cout << std::endl << "The output will be dumped and a 'SUCCESS' or ";
    std::cout << "FAIL (with reason) will be" << std::endl << "displayed ";
    std::cout << "in the terminal window." << std::endl;
}
