#include <iostream>
#include <tdrmain.h>
#include <tdrrenderer.h>

int main(int argc, char* argv[])
{
    std::string json = load_text_file("./cube.json");
    if (json.length() <= 0) {
        std::cerr << "Failed to load the JSON file." << std::endl;
        exit(EXIT_FAILURE);
    }

    /* Dump the string, just as a sanity check */
    std::cout << "====================" << std::endl;
    std::cout << "JSON Output:" << std::endl << json << std::endl;
    std::cout << "====================" << std::endl;

    std::shared_ptr<TDRMesh> mesh = TDRMesh::Load2(json.c_str(), json.length());

    size_t count = mesh->getMeshCount();
    std::cout << "Found " << count << " meshes:" << std::endl;
    for (size_t i = 0; i < count; i++) {
        std::cout << "\t- " << mesh->getData(i, TDRMesh::MeshName);
        std::cout << std::endl;
    }

    return 0;
}
