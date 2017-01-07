#include <tdrmesh.h>

TDRMesh::TDRMesh(void)
{
    m_keywords.resize(TDRMESH_UNKNOWN);

    m_keywords[TDRMESH_VERTICES]    = std::string("vertices");
    m_keywords[TDRMESH_UVS]         = std::string("uvs");
    m_keywords[TDRMESH_COLORS]      = std::string("colors");
    m_keywords[TDRMESH_NORMALS]     = std::string("normals");
    m_keywords[TDRMESH_INDICES]     = std::string("indices");
    m_keywords[TDRMESH_VSHADER]     = std::string("vshader");
    m_keywords[TDRMESH_FSHADER]     = std::string("fshader");
    m_keywords[TDRMESH_GSHADER]     = std::string("gshader");
    m_keywords[TDRMESH_TEXTURE]     = std::string("texture");
}

void TDRMesh::Delete(std::shared_ptr<TDRMesh> mesh) { }

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
#ifdef TDR_DEBUG
            std::cerr << "Found string: " << buff << std::endl;
#endif

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

std::shared_ptr<TDRMesh> TDRMesh::Load2(const char* buffer, size_t len)
{
    std::shared_ptr<TDRMesh> ret(new TDRMesh());
    struct meshobj_t tobj;

    jsmn_parser parser;
    jsmn_init(&parser);
    int count = jsmn_parse(&parser, buffer, len, nullptr, 0);

    std::stack<std::string> sstack;
    std::vector<jsmntok_t> tokens;
    tokens.resize(count);

    jsmn_init(&parser);
    if (jsmn_parse(&parser, buffer, len, tokens.data(),
        tokens.size()) != count) {
#ifdef TDR_MESH
        std::cerr << "TDRMesh::Load2(): Failed to parse due to count ";
        std::cerr << "mismatch.  Aborting." << std::endl;
#endif
        return nullptr;
    }

    for (size_t i = 1; i < tokens.size(); i++) {
        int len = tokens[i].end - tokens[i].start + 1;
        const char* tjson = buffer + tokens[i].start;
        char buff[TDRMESH_BUFF_MAXLEN];

        if (tokens[i].type == JSMN_PRIMITIVE ||
            tokens[i].type == JSMN_ARRAY ||
            tokens[i].type == JSMN_UNDEFINED) {
            continue;
        }

        if (tokens[i].type == JSMN_OBJECT) {
            if (ret->parse_object(&tobj, tjson, len)) {
#ifdef TDR_DEBUG
                std::cerr << "TDRMesh::Load2(): Failed to parse object. ";
                std::cerr << "Aborting!" << std::endl;
#endif
                return nullptr;
            }

            tobj.name = std::string(sstack.top());
            tobj.json = std::string(tjson, len);
            ret->m_objs.push_back(tobj);
            sstack.pop();

            if (ret->fast_forward(tokens, i) == 0) {
#ifdef TDR_DEBUG
                std::cerr << "TDRMesh::Load2(): Are we done?  Returning..";
                std::cerr << std::endl;
#endif
                return ret;
            } else {
                i = ret->fast_forward(tokens, i) - 1;
                continue;
            }
        }

        if (tokens[i].type == JSMN_STRING) {
            snprintf(buff, len, "%s", tjson);
            sstack.push(std::string(buff));
#ifdef TDR_DEBUG
            std::cerr << "TDRMesh::Load2(): pushing string: ";
            std::cerr << sstack.top() << std::endl;
#endif
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

std::vector<float> TDRMesh::getData(size_t index, Data type)
{
    switch (type) {
    case Color:
        return m_objs[index].colors;
    case Index:
        return m_objs[index].indeces;
    case Normal:
        return m_objs[index].normals;
    case UV:
        return m_objs[index].uvs;
    case Vertex:
        return m_objs[index].vertices;
    default:
        return std::vector<float>();
    }

    return std::vector<float>();
}

size_t TDRMesh::getMeshCount(void)
{
    return m_objs.size();
}

bool TDRMesh::hasData(size_t index, TDRMesh::Data type)
{
    switch (type) {
    case TDRMesh::Color:
        if (m_objs[index].colors.size() > 0) {
            return true;
        }
        break;
    case TDRMesh::Index:
        if (m_objs[index].indeces.size() > 0) {
            return true;
        }
        break;
    case TDRMesh::Normal:
        if (m_objs[index].normals.size() > 0) {
            return true;
        }
        break;
    case TDRMesh::UV:
        if (m_objs[index].uvs.size() > 0) {
            return true;
        }
        break;
    case TDRMesh::Vertex:
        if (m_objs[index].vertices.size() > 0) {
            return true;
        }
        break;
    default:
        return false;
    }

    return false;
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

#ifdef TDR_DEBUG
void TDRMesh::Test(const char* json, size_t len)
{
    std::cerr << "TDRMesh::Test(..) not implemented yet." << std::endl;
}
#endif

std::vector<float> TDRMesh::parse_array(const char* json, int len)
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

#ifdef TDR_DEBUG
    std::cerr << "TDRMesh::parse_array() --- Attempting to parse:";
    std::cerr << std::endl << str << std::endl;
    std::cerr << "TDRMesh::parse_array() ------------------------";
    std::cerr << std::endl;
#endif

    jsmn_init(&parser);
    if (tok_count != jsmn_parse(&parser, str, len, tokens.data(),
        tokens.size())) {
#ifdef TDR_DEBUG
        std::cerr << "TDRMesh::parse_array(): Failed to parse array. ";
        std::cerr << "Token size mismatch." << std::endl;
#endif
        return ret;
    }

    if (tokens[0].type != JSMN_ARRAY) {
#ifdef TDR_DEBUG
        std::cerr << "TDRMesh::parse_array(): Failed to parse array. ";
        std::cerr << "Array not found." << std::endl;
#endif
        return ret;
    }

    for (size_t i = 1; i < tokens.size(); i++) {
        if (tokens[i].type != JSMN_PRIMITIVE) {
#ifdef TDR_DEBUG
            std::cerr << "TDRMesh::parse_array(): Failed to parse array. ";
            std::cerr << "Expecting primitive." << std::endl;
#endif
            return ret;
        }

        memset(buff, 0, TDRMESH_BUFF_MAXLEN);
        snprintf(buff, tokens[i].end - tokens[i].start + 1, "%s",
            str + tokens[i].start);
        float temp = std::atof(buff);
#ifdef TDR_DEBUG
        std::cerr << "Parsing {" << temp << "}" << std::endl;
#endif
        ret.push_back(temp);
    }

    return ret;
}

int TDRMesh::parse_object(struct meshobj_t* obj, const char* json, int len)
{
    if (obj == nullptr) {
        return -1;
    }

    char buff[TDRMESH_BUFF_MAXLEN];
    jsmn_parser parser;
    jsmn_init(&parser);

    int count = jsmn_parse(&parser, json, len, nullptr, 0);
    std::vector<jsmntok_t> tokens;
    tokens.resize(count);

#ifdef TDR_DEBUG
    std::cerr << "TDRMesh::parse_object() --- Attempting to parse:";
    std::cerr << std::endl << json << std::endl;
    std::cerr << "TDRMesh::parse_object() ------------------------";
    std::cerr << std::endl;
#endif

    jsmn_init(&parser);
    if (count != jsmn_parse(&parser, json, len, tokens.data(),
        tokens.size())) {
#ifdef TDR_DEBUG
        std::cerr << "TDRMesh::parse_object(): Failed to parse object. ";
        std::cerr << "Token size mismatch." << std::endl;
#endif
        return 1;
    }

    std::stack<tdrmesh_types_e> tstack;
    tdrmesh_types_e etype;

    for (size_t i = 1; i < tokens.size(); i++) {
        int slen = tokens[i].end - tokens[i].start + 1;
        const char* tjson = json + tokens[i].start;

        if (tokens[i].type == JSMN_PRIMITIVE ||
            tokens[i].type == JSMN_UNDEFINED) {
            continue;
        }

        if (tokens[i].type == JSMN_OBJECT) {
#ifdef TDR_DEBUG
            std::cerr << "TDRMesh::parse_object(): Malformed TDRMesh ";
            std::cerr << "object.  Aborting!" << std::endl;
#endif
            return 1;
        }

        if (tokens[i].type == JSMN_ARRAY) {
            if (tstack.empty()) {
#ifdef TDR_DEBUG
                std::cerr << "TDRMesh::parse_object(): Found array ";
                std::cerr << "without qualifier.  Aborting!" << std::endl;
#endif
                return 1;
            }

            if (tstack.top() == TDRMESH_TEXTURE) {
#ifdef TDR_DEBUG
                std::cerr << "TDRMesh::parse_object(): Not parsing ";
                std::cerr << "texture.  Going from token ";
                std::cerr << i << " to ";
#endif
                i = fast_forward(tokens, i) - 1;
#ifdef TDR_DEBUG
                std::cerr << i << "." << std::endl;
#endif
                tstack.pop();
                continue;
            }

            switch (tstack.top()) {
            case TDRMESH_VERTICES:
                m_vertexdata = parse_array(tjson, slen);
                break;
            case TDRMESH_UVS:
                m_uvdata = parse_array(tjson, slen);
                break;
            case TDRMESH_COLORS:
                m_colordata = parse_array(tjson, slen);
                break;
            case TDRMESH_NORMALS:
                m_normals = parse_array(tjson, slen);
                break;
            case TDRMESH_INDICES:
                m_indexdata = parse_array(tjson, slen);
                break;
            default:
#ifdef TDR_DEBUG
                std::cerr << "TDRMesh::parse_object(): Malformed mesh. ";
                std::cerr << "Aborting!" << std::endl;
#endif
                return 1;
            }

            tstack.pop();
            if (fast_forward(tokens, i) != 0) {
                i = fast_forward(tokens, i) - 1;
            }
        }

        if (tokens[i].type == JSMN_STRING) {
            snprintf(buff, slen, "%s", tjson);
            etype = parse_identifier(buff, slen);
            if (etype == TDRMESH_UNKNOWN && tstack.empty()) {
#ifdef TDR_DEBUG
                std::cerr << "Failed to identify type: '" << buff;
                std::cerr << "'.  Aborting!" << std::endl;
#endif
                return 1;
            }

            if (tstack.empty()) {
#ifdef TDR_DEBUG
                std::cerr << "TDRMesh::parse_object(): Pushing ";
                std::cerr << m_keywords[etype] << std::endl;
#endif
                tstack.push(etype);
                continue;
            }

            switch (tstack.top()) {
            case TDRMESH_VSHADER:
                obj->vertex_shader = std::string(buff);
                break;
            case TDRMESH_FSHADER:
                obj->fragment_shader = std::string(buff);
                break;
            case TDRMESH_GSHADER:
                obj->geometry_shader = std::string(buff);
                break;
            default:
#ifdef TDR_DEBUG
                std::cerr << "TDRMesh::parse_object(): Weird texture. ";
                std::cerr << "Aborting!" << std::endl;
#endif
                return 1;
            }
#ifdef TDR_DEBUG
            std::cerr << "TDRMesh::parse_object(): ";
            std::cerr <<  m_keywords[tstack.top()] << " = " << buff;
            std::cerr << std::endl;
#endif
            tstack.pop();
        }
    }

    return 0;
}

TDRMesh::tdrmesh_types_e TDRMesh::parse_identifier(const char* str, size_t len)
{
    std::string tmp(str, len - 1);

    for (unsigned int i = 0; i < m_keywords.size(); i++) {
        if (m_keywords[i].length() != tmp.length()) {
            continue;
        }

        if (m_keywords[i].compare(tmp) == 0) {
            return static_cast<TDRMesh::tdrmesh_types_e>(i);
        }
    }

    return TDRMESH_UNKNOWN;
}

size_t TDRMesh::fast_forward(std::vector<jsmntok_t> tokens, size_t index)
{
    for (size_t i = index; i < tokens.size(); i++) {
        if (tokens[i].start >= tokens[index].end) {
            return i;
        }
    }

#ifdef TDR_DEBUG
    std::cerr << "TDRMesh::fast_forward(): failed." << std::endl;
#endif
    return 0;
}
