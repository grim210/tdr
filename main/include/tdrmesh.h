#ifndef TDRMAIN_TDRMESH_H
#define TDRMAIN_TDRMESH_H

#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "tinyxml2.h"

class TDRMesh {
public:
    enum DataType {
        COLOR,
        INDEX,
        VERTEX,
        UV,
    };

    TDRMesh(std::string path);
    TDRMesh(const char* data, size_t len);
    virtual ~TDRMesh(void) { };
private:
    struct mesh_t {
        std::string name;
        std::string texture;
        std::vector<std::vector<float>> m_binary;
        std::vector<DataType> m_types;
    };

    std::vector<struct mesh_t> m_mesh;
    std::shared_ptr<txml::XMLDocument> m_doc;

    static std::shared_ptr<TDRMesh> load(const char* data, size_t len);
};

#endif
