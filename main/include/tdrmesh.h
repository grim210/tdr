#ifndef TDRMAIN_TDRMESH_H
#define TDRMAIN_TDRMESH_H

#include <memory>
#include <vector>

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
    std::vector<float> has(Data type);
private:
    std::vector<float> m_colordata;
    std::vector<float> m_indexdata;
    std::vector<float> m_uvdata;
    std::vector<float> m_vertexdata;
};

#endif  /* TDRMAIN_TDRMESH_H */
