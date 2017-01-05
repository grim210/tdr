#include <tdrmesh.h>

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
    char buff[TDRMESH_BUFF_MAXLEN];

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
#ifdef TDR_DEBUG
                std::cerr << "Unrecognized character in TDRMesh::Load.  ";
                std::cerr << "Skipping..." << std::endl;
#endif
                break;
            }
            break;
        case JSMN_STRING:
            memset(buff, 0, TDRMESH_BUFF_MAXLEN);
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
#ifdef TDR_DEBUG
            std::cerr << "Found primitive or object.  Skipping.";
            std::cerr << std::endl;
#endif
            break;
        default:
#ifdef TDR_DEBUG
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
    std::vector<float> temp;
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
    default:
        return temp;
    }
}

bool TDRMesh::has(TDRMesh::Data type)
{
    switch (type) {
    case TDRMesh::Color:
        if (m_colordata.size() > 0) {
            return true;
        }
        return false;
    case TDRMesh::Index:
        if (m_indexdata.size() > 0) {
            return true;
        }
        return false;
    case TDRMesh::UV:
        if (m_uvdata.size() > 0) {
            return true;
        }
        return false;
    case TDRMesh::Vertex:
        if (m_vertexdata.size() > 0) {
            return true;
        }
        return false;
    default:
        return false;
    }
}

std::vector<float> TDRMesh::parse_array(const char* json, size_t len)
{
    int tok_count;
    char buff[TDRMESH_BUFF_MAXLEN];
    char str[len + 1];
    std::vector<float> ret;
    std::vector<jsmntok_t> tokens;

    memset(buff, 0, TDRMESH_BUFF_MAXLEN);
    memset(str, 0, len + 1);
    snprintf(str, len, "%s", json);

    jsmn_parser parser;
    jsmn_init(&parser);
    tok_count = jsmn_parse(&parser, str, len, nullptr, 0);
    tokens.resize(tok_count);

    jsmn_init(&parser);
    if (tok_count != jsmn_parse(&parser, str, len, tokens.data(),
        tokens.size())) {
#ifdef TDR_DEBUG
        std::cerr << "Failed to parse array.  Token size mismatch.";
        std::cerr << std::endl;
#endif
        return ret;
    }

    if (tokens[0].type != JSMN_ARRAY) {
#ifdef TDR_DEBUG
        std::cerr << "Failed to parse array.  Array not found.";
        std::cerr << std::endl;
#endif
        return ret;
    }

    for (size_t i = 1; i < tokens.size(); i++) {
        if (tokens[i].type != JSMN_PRIMITIVE) {
#ifdef TDR_DEBUG
            std::cerr << "Failed to parse array.  Expecting primitive.";
            std::cerr << std::endl;
#endif
            return ret;
        }

        memset(buff, 0, TDRMESH_BUFF_MAXLEN);
        snprintf(buff, tokens[i].end - tokens[i].start + 1, "%s",
            str + tokens[i].start);
        float temp = std::atof(buff);
        ret.push_back(temp);
    }

    return ret;
}
