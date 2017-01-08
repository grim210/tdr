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

/* XXX: Will be deleted as soon as Load2 is solidified. */
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
                ret->m_uvdata = ret->parse_float_array(tjson, len);
                break;
            case 'v':
                ret->m_vertexdata = ret->parse_float_array(tjson, len);
                break;
            case 'i':
                ret->m_indexdata = ret->parse_float_array(tjson, len);
                break;
            case 'n':
                ret->m_normals = ret->parse_float_array(tjson, len);
                break;
            case 'c':
                ret->m_colordata = ret->parse_float_array(tjson, len);
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

/* XXX: Remove */
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

/* XXX: Remove */
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
#ifdef TDR_DEBUG
        std::cerr << "TDRMesh::getData(): Requested invalid data type.";
        std::cerr << std::endl;
#endif
    }

    return std::vector<float>();
}

/* In this function, we focus on loading a Mesh from the standpoint of
* multiple objects per mesh.  Meaning that if we have two objects (for
* instance a cube and a sphere) that the Load2 function would simply look
* deep enough to understand that there are 2 objects, and that parse_object
* needs to be called for each of those. */
std::shared_ptr<TDRMesh> TDRMesh::Load2(const char* buffer, size_t len)
{
    /* The mesh that will be returned, plus a temporary object that will
    * be used to add to the mesh object vector. */
    std::shared_ptr<TDRMesh> ret(new TDRMesh());
    struct meshobj_t tobj;

    /* the first call to the parser is to count how many tokesn are contained
    * in the mesh file.  Every single piece of data is considered a token. So
    * an array is a token, and all the pieces of data inside the array are
    * also tokens.  That number of tokens is stored so that we can allocate
    * enough memory to hold them all. */
    jsmn_parser parser;
    jsmn_init(&parser);
    int count = jsmn_parse(&parser, buffer, len, nullptr, 0);

    /* The array of tokens, resized to hold enough tokens for the entire
    * JSON mesh file. */
    std::vector<jsmntok_t> tokens;
    tokens.resize(count);

    /* This string stack is used to store strings, so that we have a bit of
    * context when we're trying to parse objects.  Each object is named, so
    * when we find an object, we look on the stack to see what that object
    * was called. */
    std::stack<std::string> sstack;

    /* Re-initialize the parser, and this time, actually parse the file */
    jsmn_init(&parser);
    if (jsmn_parse(&parser, buffer, len, tokens.data(),
        /* if we get a mismatch, some crazy stuff has happened. */
        tokens.size()) != count) {
#ifdef TDR_MESH
        std::cerr << "TDRMesh::Load2(): Failed to parse due to count ";
        std::cerr << "mismatch.  Aborting." << std::endl;
#endif
        return nullptr;
    }

    /* We start our index at one because every TDRMesh file must be a
    * JSON object, which jsmn will happily parse and inform you of. */
    size_t i = 1;
    while (i < tokens.size()) {
        int len = tokens[i].end - tokens[i].start + 1;
        const char* tjson = buffer + tokens[i].start;
        char buff[TDRMESH_BUFF_MAXLEN];

        /* If we find this stuff in the root object, it can't be used, but
        * I'm not going to keep the parsing code that strict. */
        if (tokens[i].type == JSMN_PRIMITIVE ||
            tokens[i].type == JSMN_ARRAY ||
            tokens[i].type == JSMN_UNDEFINED) {
            i++;
            continue;
        }

        /* If we find an object, attempt to parse it. */
        if (tokens[i].type == JSMN_OBJECT) {
            if (ret->parse_object(&tobj, tjson, len)) {
#ifdef TDR_DEBUG
                std::cerr << "TDRMesh::Load2(): Failed to parse object. ";
                std::cerr << "Aborting!" << std::endl;
#endif
                return nullptr;
            }

            /* If we make it this far, the object has been successfully parsed
            * and we just need to ensure that the data structure is completely
            * filled with all the necessary info. */
            tobj.name = std::string(sstack.top());
            tobj.json = std::string(tjson, len);
            ret->m_objs.push_back(tobj);

            /* After parsing, we can remove the object name from the stack. */
            sstack.pop();

            /* Now we must fast forward, to the next top-level object.  If
            * this isn't done, this method will start looking down into the
            * objects and seeing the same data that has already been parsed. */
            if (ret->fast_forward(tokens, i) == 0) {
#ifdef TDR_DEBUG
                std::cerr << "TDRMesh::Load2(): Cannot fast forward.  ";
                std::cerr << "Returning.." << std::endl;
#endif
                return ret;
            } else {
                i = ret->fast_forward(tokens, i);
                continue;
            }
        }

        /* If we find a string, it's probably the name of an object.  If we
        * find it inside the root object, there are some problems with the
        * mesh file.  But that isn't checked for. */
        if (tokens[i].type == JSMN_STRING) {
            snprintf(buff, len, "%s", tjson);
            sstack.push(std::string(buff));
#ifdef TDR_DEBUG
            std::cerr << "TDRMesh::Load2(): pushing string: ";
            std::cerr << sstack.top() << std::endl;
#endif
        }

        i++;
    }

    return ret;
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


#ifdef TDR_DEBUG
void TDRMesh::Test(const char* json, size_t len)
{
    std::cerr << "TDRMesh::Test(..) not implemented yet." << std::endl;
}
#endif

std::vector<float> TDRMesh::parse_float_array(const char* json, int len)
{
    int count;
    char buff[TDRMESH_BUFF_MAXLEN];
    char str[len + 1];
    std::vector<float> ret;

    if (json == NULL || len <= 0) {
#ifdef TDR_DEBUG
        std::cerr << "TDRMesh::parse_float_array(): Invalid parameters ";
        std::cerr << "passed.  Returning empty vector." << std::endl;
#endif
        return ret;
    }

    /* First, we clear our buffers, and then copy the parameter string into
    * a seperate object so we don't accidently trample it. */
    memset(buff, 0, TDRMESH_BUFF_MAXLEN);
    memset(str, 0, len + 1);
    snprintf(str, len, "%s", json);

#ifdef TDR_DEBUG
    /* XXX: To verbose? */
    std::cerr << "TDRMesh::parse_float_array() --- Attempting to parse:";
    std::cerr << std::endl << str << std::endl;
    std::cerr << "TDRMesh::parse_float_array() ------------------------";
    std::cerr << std::endl;
#endif

    /* Parse the object initially just to get a token count. */
    jsmn_parser parser;
    jsmn_init(&parser);
    count = jsmn_parse(&parser, str, len, nullptr, 0);

    /* Resize our token buffer to hold everything we need to parse. */
    std::vector<jsmntok_t> tokens;
    tokens.resize(count);

    /* Actually parse the string this time, but still check to see if
    * something got dorked up in the process.. You never know. */
    jsmn_init(&parser);
    if (count != jsmn_parse(&parser, str, len, tokens.data(),
        tokens.size())) {
#ifdef TDR_DEBUG
        std::cerr << "TDRMesh::parse_float_array(): Token size mismatch;";
        std::cerr << "failed to parse array." << std::endl;
#endif
        return ret;
    }

    /* Check if the root token is the array; that's what we're expecting. */
    if (tokens[0].type != JSMN_ARRAY) {
#ifdef TDR_DEBUG
        std::cerr << "TDRMesh::parse_float_array(): Malformed array. ";
        std::cerr << std::endl;
#endif
        return ret;
    }

    for (size_t i = 1; i < tokens.size(); i++) {
        if (tokens[i].type != JSMN_PRIMITIVE) {
#ifdef TDR_DEBUG
            std::cerr << "TDRMesh::parse_float_array(): Failed to parse ";
            std::cerr << "array.  Expecting primitive." << std::endl;
#endif
            return ret;
        }

        memset(buff, 0, TDRMESH_BUFF_MAXLEN);
        snprintf(buff, tokens[i].end - tokens[i].start + 1, "%s",
            str + tokens[i].start);

        float temp = std::atof(buff);
        ret.push_back(temp);
    }

#ifdef TDR_DEBUG
    std::cerr << "TDRMesh::parse_float_array(): Parsed " << ret.size();
    std::cerr << " float values: {";
    for (size_t i = 0; i < ret.size(); i++) {
        std::cerr << " " << ret[i] << ",";
    }
    std::cerr << " }" << std::endl;
#endif

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
                m_vertexdata = parse_float_array(tjson, slen);
                break;
            case TDRMESH_UVS:
                m_uvdata = parse_float_array(tjson, slen);
                break;
            case TDRMESH_COLORS:
                m_colordata = parse_float_array(tjson, slen);
                break;
            case TDRMESH_NORMALS:
                m_normals = parse_float_array(tjson, slen);
                break;
            case TDRMESH_INDICES:
                m_indexdata = parse_float_array(tjson, slen);
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
