#include "meshgenerator.h"

MeshGenerator::MeshGenerator(void)
{
    m_doc = std::unique_ptr<txml::XMLDocument>(new txml::XMLDocument());
}

MeshGenerator::~MeshGenerator(void) { }

std::string MeshGenerator::print(void)
{
    return std::string("<scene>\n</scene>");
}
