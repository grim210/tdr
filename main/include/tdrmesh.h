#ifndef TDRMAIN_TDRMESH_H
#define TDRMAIN_TDRMESH_H

#ifdef TDR_DEBUG
#include <iostream>
#endif

#include <cstring>
#include <memory>
#include <string>
#include <vector>

#include <jsmn.h>

class TDRMesh {
public:
    enum Data {
        Color,
        Index,
        UV,
        Vertex
    };
    TDRMesh(void) { };
    virtual ~TDRMesh(void) { };
    static std::shared_ptr<TDRMesh> Cube(void);
    static void Delete(std::shared_ptr<TDRMesh> mesh);
    static std::shared_ptr<TDRMesh> Load(const char* buff, size_t len);
    std::vector<float> get(Data type);
    bool has(Data type);
private:
    std::vector<float> m_colordata;
    std::vector<float> m_indexdata;
    std::vector<float> m_normals;
    std::vector<float> m_uvdata;
    std::vector<float> m_vertexdata;

    std::vector<float> parse_array(const char* json, size_t len);
};

#endif  /* TDRMAIN_TDRMESH_H */
