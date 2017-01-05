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

#define TDRMESH_BUFF_MAXLEN        (256)

class TDRMesh {
public:
    enum Data {
        Color,
        Index,
        Normal,
        UV,
        Vertex
    };
    TDRMesh(void) { };
    virtual ~TDRMesh(void) { };
    static void Delete(std::shared_ptr<TDRMesh> mesh);
    static std::shared_ptr<TDRMesh> Load(const char* buff, size_t len);
    static std::shared_ptr<TDRMesh> Load2(const char* buff, size_t len);

    std::vector<float> get(Data type);
    bool has(Data type);

    size_t getMeshCount(void);
    std::vector<float> getData(size_t index, Data type);
    std::string getMeshName(size_t index);
    bool hasData(size_t index, Data type);

#ifdef TDR_DEBUG
    /* Call this from a debug build to get a very verbose output to verify
     * whether the provided buffer is a valid mesh object or not. */
    static void Test(const char* json, size_t len);
#endif
private:
    struct meshobj_t {
        std::string name;
        std::string json;

        std::string vertex_shader;
        std::string fragment_shader;
        std::vector<float> colors;
        std::vector<float> indeces;
        std::vector<float> normals;
        std::vector<float> uvs;
        std::vector<float> vertices;
    };

    std::vector<struct meshobj_t> m_objs;
    std::vector<float> m_colordata;
    std::vector<float> m_indexdata;
    std::vector<float> m_normals;
    std::vector<float> m_uvdata;
    std::vector<float> m_vertexdata;

    std::vector<float> parse_array(const char* json, size_t len);
    int parse_object(struct meshobj_t* obj, const char* json, size_t len);
};

#endif  /* TDRMAIN_TDRMESH_H */
