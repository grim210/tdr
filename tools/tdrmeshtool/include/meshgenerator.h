#ifndef TDRMESHTOOL_MESHGENERATOR_H
#define TDRMESHTOOL_MESHGENERATOR_H

#include <iostream>
#include <tdrmain.h>

class MeshGenerator {
public:
    enum Simple {
        Cube,
        Sphere,
    };

    MeshGenerator(void);
    virtual ~MeshGenerator(void);
    void generate(MeshGenerator::Simple object);
    void interactive(void);
private:
};

#endif
