#ifndef RENDERER_TEXTURE_H
#define RENDERER_TEXTURE_H

#ifdef TDR_DEBUG
#include <iostream>
#endif

#include <string>
#include <vector>

class Texture {
public:
    virtual ~Texture(void) { };
    virtual std::string getTextureType(void) = 0;
    virtual std::vector<char> getPixelData(void) = 0;
    virtual uint32_t getMipMapCount(void) = 0;
    virtual uint32_t getWidth(void) = 0;
    virtual uint32_t getHeight(void) = 0;
    virtual bool isValid(void) = 0;
};

#endif /* RENDERER_TEXTURE_H */
