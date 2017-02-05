#include "tdrmesh.h"
#include "miniz.h"

TDRMesh::TDRMesh(std::string path)
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
    length = 0;
}

TDRMesh::TDRMesh(const char* data, size_t len)
{
    std::shared_ptr<TDRMesh> mesh;

    this->m_doc = mesh->m_doc;
}

std::shared_ptr<TDRMesh> TDRMesh::load(const char* data, size_t len)
{
    std::shared_ptr<TDRMesh> ret;
    std::shared_ptr<txml::XMLDocument> doc;

    ret->m_doc = std::shared_ptr<txml::XMLDocument>(new txml::XMLDocument());
    return ret;
}
