#ifndef TDRMESHTOOL_HELP_H
#define TDRMESHTOOL_HELP_H

#include <cstring>
#include <iostream>
#include <string>

#include "config.h"

class Help {
public:
    static void Standard(void);
    static void Extended(std::string option);
private:
    static void Convert(void);
    static void Generate(void);
    static void Validate(void);
};

#endif /* TDRMESHTOOL_HELP_H */
