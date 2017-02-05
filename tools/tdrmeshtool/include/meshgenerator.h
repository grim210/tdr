#ifndef TDRMESHTOOL_MESHGENERATOR_H
#define TDRMESHTOOL_MESHGENERATOR_H

#include <iostream>
#include <memory>

#include <tdrmain.h>

class MeshGenerator {
public:
    enum Simple {
        Cube,
        Sphere,
    };

    MeshGenerator(void);
    virtual ~MeshGenerator(void);
    /* TODO: Just as a proof of concept while I'm working this, I'll have the
    * MeshGenerator simply generate my cube in the constructor.
    void generate(MeshGenerator::Simple object);
    void interactive(void); */
    void dump(void);
private:
    std::unique_ptr<txml::XMLDocument> m_doc;
    std::vector<std::vector<float>> m_bdata;
    std::vector<std::string> m_bnames;
};

#endif
