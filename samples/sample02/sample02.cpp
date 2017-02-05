#include <iostream>
#include <tdrmain.h>

int main(int argc, char* argv[])
{
    std::string path;

    if (argc == 1) {
        path = "scene.xml";
    } else {
        path = argv[1];
    }

    txml::XMLDocument* doc = new txml::XMLDocument();
    doc->LoadFile(path.c_str());
    doc->Print();

    delete(doc);

    return 0;
}
