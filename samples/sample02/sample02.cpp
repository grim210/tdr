#include <iostream>
#include <tdrmain.h>

int main(int argc, char* argv[])
{
    std::shared_ptr<TDRMesh> mesh;
    std::string path;

    if (argc == 1) {
        path = "scene.xml";
    } else {
        path = argv[1];
    }

    mesh = TDRMesh::Load(path);
#ifdef TDR_DEBUG
    mesh->_dump();
#endif

    return 0;
}
