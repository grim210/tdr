#include <tdrmain.h>

std::string load_text_file(std::string path)
{
    std::fstream fs(path, std::ifstream::in);
    if (!fs.is_open()) {
        return std::string("");
    }

    fs.seekg(0, fs.end);
    size_t len = fs.tellg();
    fs.seekg(0, fs.beg);
    std::unique_ptr<char> buff(new char[len + 1]);

    memset(buff.get(), 0, len + 1);
    fs.read(buff.get(), len);

    return std::string(buff.get());
}

void tdrmain_version(char* buff, size_t len)
{
    memset(buff, 0, len);
    snprintf(buff, len, "%d.%d.%d", TDRMAIN_VERSION_MAJOR,
        TDRMAIN_VERSION_MINOR, TDRMAIN_VERSION_PATCH);
}
