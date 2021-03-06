#include "tdrmesh.h"
#include "miniz.h"

std::shared_ptr<TDRMesh> TDRMesh::Load(std::string path)
{
    std::shared_ptr<TDRMesh> mesh;
    std::fstream fp;
    char* buffer;
    size_t length;

    fp.open(path, std::fstream::in | std::fstream::binary);
    fp.seekg(0L, std::fstream::end);
    length = fp.tellg();
    fp.seekg(0L, std::fstream::beg);

    buffer = new char[length];
    fp.read(buffer, length);
    fp.close();

    mesh = TDRMesh::load(buffer, length);
    delete[](buffer);

    return mesh;
}

std::shared_ptr<TDRMesh> TDRMesh::Load(const char* data, size_t len)
{
    std::shared_ptr<TDRMesh> mesh;

    mesh = TDRMesh::load(data, len);
    return mesh;
}

std::shared_ptr<TDRMesh> TDRMesh::load(const char* data, size_t len)
{
    std::shared_ptr<TDRMesh> ret(new TDRMesh());

    std::shared_ptr<txml::XMLDocument> doc(new txml::XMLDocument());
    doc->Parse(data, len);

#ifdef TDR_DEBUG
    std::cout << "TDRMesh::load(const char*, size_t) ";
    std::cout << "XML parse complete." << std::endl;
#endif

    txml::XMLElement* mesh;
    mesh = doc->FirstChildElement("scene");
    if (!mesh) {
#ifdef TDR_DEBUG
        std::cerr << "TDRMesh::load(const char*, size_t) ";
        std::cerr << "Failed to parse <scene> tag from mesh xml." << std::endl;
#endif
    }

    mesh = mesh->FirstChildElement("mesh");
    if (!mesh) {
#ifdef TDR_DEBUG
        std::cerr << "TDRMesh::load(const char*, size_t) ";
        std::cerr << "Failed to parse <mesh> tag from <scene>." << std::endl;
#endif
    }

    struct mesh_t m;
    struct bindata_t bin;
    m.name = mesh->Attribute("name");
    txml::XMLElement* obj;

    obj = mesh->FirstChildElement("vertices");
    if (!obj) {
#ifdef TDR_DEBUG
        std::cout << "TDRMesh::load(const char*, size_t) ";
        std::cout << "Didn't find vertices." << std::endl;
#endif
    } else {
        std::memset(&bin, 0, sizeof(struct bindata_t));
        ret->load_bindata(&bin, obj);
        bin.type = TDRMesh::DataType::VERTEX;
        m.m_bindata.push_back(bin);
    }

    obj = mesh->FirstChildElement("colors");
    if (!obj) {
#ifdef TDR_DEBUG
        std::cout << "TDRMesh::load(const char*, size_t) ";
        std::cout << "Didn't find color data." << std::endl;
#endif
    } else {
        std::memset(&bin, 0, sizeof(struct bindata_t));
        ret->load_bindata(&bin, obj);
        bin.type = TDRMesh::DataType::COLOR;
        m.m_bindata.push_back(bin);
    }

    /* XXX: Check for other data types as well */

    obj = mesh->FirstChildElement("shaders");
    if (!obj) {
#ifdef TDR_DEBUG
        std::cout << "Failed to find shaders." << std::endl;
#endif
    } else {
        m.m_shaderdata = ret->load_shaders(obj);
    }

    ret->m_doc = doc;
    ret->m_mesh.push_back(m);

    return ret;
}

bool TDRMesh::load_bindata(TDRMesh::bindata_t* bin, txml::XMLElement* tag)
{
    std::fstream fp;
    const char* result = nullptr;
    char* buff;
    size_t len;

    result = tag->Attribute("per");
    bin->per = static_cast<size_t>(strtol(result, nullptr, 10));

    result = tag->Attribute("count");
    fp.open(tag->GetText(), std::fstream::in | std::fstream::binary);
    if (!fp.is_open()) {
        return false;
    }

    fp.seekg(0L, fp.end);
    len = fp.tellg();
    fp.seekg(0L, fp.beg);

    buff = new char[len];
    std::memset(buff, 0, len);

    bin->m_data = load_floats(buff, len);
    delete[](buff);

    if (bin->m_data.size() == 0) {
        return false;
    }

    return true;
}

std::vector<TDRMesh::shaderdata_t> TDRMesh::load_shaders(txml::XMLElement* tag)
{
    std::vector<TDRMesh::shaderdata_t> ret;
    txml::XMLElement* element;
    TDRMesh::shaderdata_t tmp;
    std::fstream fp;
    char* buff;

    element = tag->FirstChildElement();
    while (element != nullptr) {
        std::memset(&tmp, 0, sizeof(struct shaderdata_t));
        tmp.type = element->Attribute("type");

        std::fstream fp(element->GetText(),
            std::fstream::in | std::fstream::binary);
        if (!fp.is_open()) {
#ifdef TDR_DEBUG
            std::cout << "Failed to open file '" << element->GetText();
            std::cout << std::endl;
#endif
            break;
        }

        fp.seekg(0L, fp.end);
        size_t len = fp.tellg();
        fp.seekg(0L, fp.beg);

        if (len == 0) {
#ifdef TDR_DEBUG
            std::cout << "Found invalid file while loading shader source. ";
            std::cout << "Skipping..." << std::endl;
#endif
            element = element->NextSiblingElement();
            continue;
        }

        buff = new char[len];
        fp.read(buff, len);
        tmp.source = std::string(buff);

        delete[](buff);
        fp.close();

        ret.push_back(tmp);
        element = element->NextSiblingElement();
    }

    return ret;
}

std::vector<float> TDRMesh::load_floats(const char* data, size_t len)
{
    std::vector<float> ret;

    /* if it's not a multiple of 4, the data is invalid.  We're dealing
    * with data that is 32 bits wide. */
    if (len % 4 != 0) {
        return ret;
    }

    float tmp;
    for (size_t i = 0; i < len; i+=4) {
        std::memcpy(&tmp, data + i, 4);
        ret.push_back(tmp);
    }

    return ret;
}

#ifdef TDR_DEBUG
void TDRMesh::_dump(void)
{
    std::cout << "TDRMesh::_dump(void) Found ";
    std::cout << m_mesh.size() << " mesh(es):" << std::endl;

    for (size_t i = 0; i < m_mesh.size(); i++) {
        std::cout << "TDRMesh::_dump(void) Mesh[" << i;
        std::cout << "] name: " << m_mesh[i].name;
        std::cout << std::endl;
        std::cout << "TDRMesh::_dump(void)\tFound ";
        std::cout << m_mesh[i].m_bindata.size() << " sets of vertex data:";
        std::cout << std::endl;
        for (size_t j = 0; j < m_mesh[i].m_bindata.size(); j++) {
            std::cout << "TDRMesh::_dump(void)\t\tBuffer[" << j << "] ";
            std::cout << "type: ";
            switch (m_mesh[i].m_bindata[j].type) {
            case TDRMesh::DataType::COLOR:
                std::cout << "color ";
                break;
            case TDRMesh::DataType::INDEX:
                std::cout << "index ";
                break;
            case TDRMesh::DataType::VERTEX:
                std::cout << "vertex ";
                break;
            case TDRMesh::DataType::UV:
                std::cout << "UV ";
                break;
            default:
                std::cout << "**UNKOWN** ";
            }

            std::cout << "\t-- " << m_mesh[i].m_bindata[j].m_data.size();
            std::cout << " elements." << std::endl;
        }

        std::cout << "TDRMesh::_dump(void)\tFound ";
        std::cout << m_mesh[i].m_shaderdata.size() << " shaders:" << std::endl;
        for (size_t j = 0; j < m_mesh[i].m_shaderdata.size(); j++) {
            std::cout << "TDRMesh::_dump(void)\t\tShader[" << j << "] ";
            std::cout << "type: " << m_mesh[i].m_shaderdata[j].type;
            std::cout << std::endl;
        }
    }
}
#endif
