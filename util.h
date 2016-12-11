#ifndef RENDERER_UTIL_H
#define RENDERER_UTIL_H

#ifdef RENDERER_DEBUG
#include <iostream>
#endif

#include <cstring>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include <glad/glad.h>

namespace util {
std::string load_text_file(std::string path);

class DDSTexture {
public:
    enum Level {
        DXT1 = 0x31545844,
        DXT3 = 0x33545844,
        DXT5 = 0x35545844
    };

    DDSTexture(std::string path);
    virtual ~DDSTexture(void) { };
    bool ready(void);
    std::vector<char> getData(void);
    Level getFormat(void);
    uint32_t getMipMapCount(void);
    uint32_t getWidth(void);
    uint32_t getHeight(void);
private:
    bool m_ready;
    uint32_t m_height;
    uint32_t m_width;
    uint32_t m_linearsize;
    uint32_t m_mmcount;
    uint32_t m_fourcc;
    Level m_level;
    std::vector<char> m_buffer;

    bool load(char* buff, size_t len);
};
}

#endif /* RENDERER_UTIL_H */
