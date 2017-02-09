#ifndef TDRMAIN_TDRMESH_H
#define TDRMAIN_TDRMESH_H

#ifdef TDR_DEBUG
  #include <iostream>
#endif

#include <fstream>
#include <memory>
#include <string>
#include <sstream>
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

    TDRMesh(void) { };
    virtual ~TDRMesh(void) { };
    static std::shared_ptr<TDRMesh> Load(std::string path);
    static std::shared_ptr<TDRMesh> Load(const char* data, size_t len);

#ifdef TDR_DEBUG
    void _dump(void);
#endif
private:
    struct bindata_t {
        size_t per;
        std::vector<float> m_data;
        enum DataType type;
    };

    struct shaderdata_t {
        std::string type;
        std::string source;
    };

    struct mesh_t {
        std::string name;
        std::string texture;
        std::vector<struct bindata_t> m_bindata;
        std::vector<struct shaderdata_t> m_shaderdata;
    };

    std::vector<struct mesh_t> m_mesh;
    std::shared_ptr<txml::XMLDocument> m_doc;
    static std::shared_ptr<TDRMesh> load(const char* data, size_t len);
    bool load_bindata(struct bindata_t* bin, txml::XMLElement* tag);
    std::vector<struct shaderdata_t> load_shaders(txml::XMLElement* tag);
    std::vector<float> load_floats(const char* data, size_t len);
};

#endif
