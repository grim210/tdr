#ifndef TDRMAIN_TDRMESH_H
#define TDRMAIN_TDRMESH_H

#ifdef TDR_DEBUG
#include <iostream>
#endif

#include <cstring>
#include <memory>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

#include <jsmn.h>

#define TDRMESH_BUFF_MAXLEN        (256)

class TDRMesh {
public:
    enum FloatData {
        Color,
        Index,
        Normal,
        UV,
        Vertex
    };

    enum StringData {
        MeshName,
        MeshJSON,
        TextureType,
        TexturePath,
        FragmentShaderPath,
        GeometryShaderPath,
        VertexShaderPath
    };

    TDRMesh(void);
    virtual ~TDRMesh(void) { };
    static void Delete(std::shared_ptr<TDRMesh> mesh);
    static std::shared_ptr<TDRMesh> Load(const char* buff, size_t len);
    static std::shared_ptr<TDRMesh> Load2(const char* buff, size_t len);

    std::vector<float> get(FloatData type);
    bool has(FloatData type);

    size_t getMeshCount(void);
    std::vector<float> getData(size_t index, FloatData type);
    std::string getData(size_t index, StringData type);
    bool hasData(size_t index, FloatData type);
    bool hasData(size_t index, StringData type);

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
        std::string geometry_shader;
        std::string texture_path;
        std::string texture_type;

        std::vector<float> colors;
        std::vector<float> indeces;
        std::vector<float> normals;
        std::vector<float> uvs;
        std::vector<float> vertices;
    };

    enum tdrmesh_types_e {
        // Array types
        TDRMESH_VERTICES,
        TDRMESH_UVS,
        TDRMESH_COLORS,
        TDRMESH_NORMALS,
        TDRMESH_INDICES,

        // Texture types.. this could expand later
        TDRMESH_VSHADER,
        TDRMESH_FSHADER,
        TDRMESH_GSHADER,

        // Other identifiers
        TDRMESH_TEXTURE,
        TDRMESH_UNKNOWN
    };

    std::unique_ptr<txml::XMLDocument> m_doc;

    std::vector<struct meshobj_t> m_objs;
    std::vector<float> m_colordata;
    std::vector<float> m_indexdata;
    std::vector<float> m_normals;
    std::vector<float> m_uvdata;
    std::vector<float> m_vertexdata;

    // stores string representations of the tdrmesh_stack_e 
    std::vector<std::string> m_keywords;

    std::vector<float> parse_float_array(const char* json, int len);
    std::vector<std::string> parse_string_array(const char* json, int len);
    int parse_object(struct meshobj_t* obj, const char* json, int len);
    tdrmesh_types_e parse_identifier(const char* str, size_t len);
    size_t fast_forward(std::vector<jsmntok_t> tokens, size_t index);
};

#endif  /* TDRMAIN_TDRMESH_H */
