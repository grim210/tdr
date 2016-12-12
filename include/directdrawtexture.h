#ifndef RENDERER_DIRECTDRAWTEXTURE_H
#define RENDERER_DIRECTDRAWTEXTURE_H

#ifdef RENDERER_DEBUG
#include <iostream>
#endif

#include <cstring>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include <glad/glad.h>

#include <texture.h>

class DirectDrawTexture : public Texture {
public:
    DirectDrawTexture(std::string path);
    virtual ~DirectDrawTexture(void) { };
    std::string getTextureType(void);
    std::vector<char> getPixelData(void);
    uint32_t getMipMapCount(void);
    uint32_t getWidth(void);
    uint32_t getHeight(void);
    bool isValid(void);
private:
    bool m_ready;
    uint32_t m_height;
    uint32_t m_width;
    uint32_t m_linearsize;
    uint32_t m_mmcount;
    uint32_t m_fourcc;
    std::vector<char> m_buffer;

    enum Level {
       DXT1 = 0x31545844,
       DXT3 = 0x33545844,
       DXT5 = 0x35545844
    } m_level;

    bool load(char* buff, size_t len);
};

#endif /* RENDERER_UTIL_H */
