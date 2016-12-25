#include <tdrmesh.h>

#define _TDRMESH_BUFF_LEN       (256)

std::shared_ptr<TDRMesh> TDRMesh::Cube(void)
{
    std::shared_ptr<TDRMesh> mesh(new TDRMesh());

    static const float g_vertex_buffer_data[] = {
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    static const float g_uv_buffer_data[] = {
        0.000059f, 1.0f-0.000004f,
        0.000103f, 1.0f-0.336048f,
        0.335973f, 1.0f-0.335903f,
        1.000023f, 1.0f-0.000013f,
        0.667979f, 1.0f-0.335851f,
        0.999958f, 1.0f-0.336064f,
        0.667979f, 1.0f-0.335851f,
        0.336024f, 1.0f-0.671877f,
        0.667969f, 1.0f-0.671889f,
        1.000023f, 1.0f-0.000013f,
        0.668104f, 1.0f-0.000013f,
        0.667979f, 1.0f-0.335851f,
        0.000059f, 1.0f-0.000004f,
        0.335973f, 1.0f-0.335903f,
        0.336098f, 1.0f-0.000071f,
        0.667979f, 1.0f-0.335851f,
        0.335973f, 1.0f-0.335903f,
        0.336024f, 1.0f-0.671877f,
        1.000004f, 1.0f-0.671847f,
        0.999958f, 1.0f-0.336064f,
        0.667979f, 1.0f-0.335851f,
        0.668104f, 1.0f-0.000013f,
        0.335973f, 1.0f-0.335903f,
        0.667979f, 1.0f-0.335851f,
        0.335973f, 1.0f-0.335903f,
        0.668104f, 1.0f-0.000013f,
        0.336098f, 1.0f-0.000071f,
        0.000103f, 1.0f-0.336048f,
        0.000004f, 1.0f-0.671870f,
        0.336024f, 1.0f-0.671877f,
        0.000103f, 1.0f-0.336048f,
        0.336024f, 1.0f-0.671877f,
        0.335973f, 1.0f-0.335903f,
        0.667969f, 1.0f-0.671889f,
        1.000004f, 1.0f-0.671847f,
        0.667979f, 1.0f-0.335851f
    };

    for (size_t i = 0; i < sizeof(g_vertex_buffer_data) / sizeof(float); i++) {
        mesh->m_vertexdata.push_back(g_vertex_buffer_data[i]);
    }

    for (size_t i = 0; i < sizeof(g_uv_buffer_data) / sizeof(float); i++) {
        mesh->m_uvdata.push_back(g_uv_buffer_data[i]);
    }

    return mesh;
}

void TDRMesh::Delete(std::shared_ptr<TDRMesh> mesh)
{

}

std::shared_ptr<TDRMesh> TDRMesh::Load(const char* buffer, size_t len)
{
    std::shared_ptr<TDRMesh> ret(new TDRMesh());

    jsmn_parser parser;
    jsmn_init(&parser);
    int count = jsmn_parse(&parser, buffer, len, nullptr, 0);

    std::vector<jsmntok_t> tokens;
    tokens.resize(count);

    jsmn_init(&parser);
    count = jsmn_parse(&parser, buffer, len, tokens.data(), tokens.size());

    char type = '\0';
    char buff[_TDRMESH_BUFF_LEN];

    for (size_t i = 0; i < tokens.size(); i++) {
        int len = tokens[i].end - tokens[i].start + 1;
        const char* tjson = nullptr;

        switch (tokens[i].type) {
        case JSMN_ARRAY:
            tjson = buffer + tokens[i].start;
            switch (type) {
            case 'u':
                ret->m_uvdata = ret->parse_array(tjson, len);
                break;
            case 'v':
                ret->m_vertexdata = ret->parse_array(tjson, len);
                break;
            case 'i':
                ret->m_indexdata = ret->parse_array(tjson, len);
                break;
            case 'n':
                ret->m_normals = ret->parse_array(tjson, len);
                break;
            case 'c':
                ret->m_colordata = ret->parse_array(tjson, len);
                break;
            default:
#ifdef RENDERER_DEBUG
                std::cerr << "Unrecognized character in TDRMesh::Load.  ";
                std::cerr << "Skipping..." << std::endl;
#endif
                break;
            }
            break;
        case JSMN_STRING:
            memset(buff, 0, _TDRMESH_BUFF_LEN);
            tjson = buffer + tokens[i].start;
            snprintf(buff, len, "%s", tjson);

            if (strncmp(buff, "vertices", strlen("vertices")) == 0) {
                type = 'v';
            } else if (strncmp(buff, "uvs", strlen("uvs")) == 0) {
                type = 'u';
            } else if (strncmp(buff, "colors", strlen("colors")) == 0) {
                type = 'c';
            } else if (strncmp(buff, "normals", strlen("normals")) == 0) {
                type = 'n';
            } else if (strncmp(buff, "indices", strlen("indices")) == 0) {
                type = 'i';
            } else {
                type = '\0';
            }
            break;
        case JSMN_PRIMITIVE:
        case JSMN_OBJECT:
#ifdef RENDERER_DEBUG
            std::cerr << "Found primitive or object.  Skipping.";
            std::cerr << std::endl;
#endif
            break;
        default:
#ifdef RENDERER_DEBUG
            std::cerr << "Unrecognized JSON token found.  Skipping.";
            std::cerr << std::endl;
#endif
            break;
        }
    }

    return ret;
}

std::vector<float> TDRMesh::get(TDRMesh::Data type)
{
    switch (type) {
    case TDRMesh::Color:
        return m_colordata;
        break;
    case TDRMesh::Index:
        return m_indexdata;
        break;
    case TDRMesh::UV:
        return m_uvdata;
        break;
    case TDRMesh::Vertex:
        return m_vertexdata;
        break;
    }
}

bool TDRMesh::has(TDRMesh::Data type)
{
    /* TODO:
     * Elaborate swtich statement like in the get() method. */
    return false;
}

std::vector<float> TDRMesh::parse_array(const char* json, size_t len)
{
    int tok_count;
    char buff[_TDRMESH_BUFF_LEN];
    char str[len + 1];
    std::vector<float> ret;
    std::vector<jsmntok_t> tokens;

    memset(buff, 0, _TDRMESH_BUFF_LEN);
    memset(str, 0, len + 1);
    snprintf(str, len, "%s", json);

    jsmn_parser parser;
    jsmn_init(&parser);
    tok_count = jsmn_parse(&parser, str, len, nullptr, 0);
    tokens.resize(tok_count);

    jsmn_init(&parser);
    if (tok_count != jsmn_parse(&parser, str, len, tokens.data(),
        tokens.size())) {
#ifdef RENDERER_DEBUG
        std::cerr << "Failed to parse array.  Token size mismatch.";
        std::cerr << std::endl;
#endif
        return ret;
    }

    if (tokens[0].type != JSMN_ARRAY) {
#ifdef RENDERER_DEBUG
        std::cerr << "Failed to parse array.  Array not found.";
        std::cerr << std::endl;
#endif
        return ret;
    }

    for (size_t i = 1; i < tokens.size(); i++) {
        if (tokens[i].type != JSMN_PRIMITIVE) {
#ifdef RENDERER_DEBUG
            std::cerr << "Failed to parse array.  Expecting primitive.";
            std::cerr << std::endl;
#endif
            return ret;
        }

        memset(buff, 0, _TDRMESH_BUFF_LEN);
        snprintf(buff, tokens[i].end - tokens[i].start + 1, "%s",
            str + tokens[i].start);
        float temp = std::atof(buff);
        ret.push_back(temp);
    }

    return ret;
}
