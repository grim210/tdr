#include "directdrawtexture.h"

DirectDrawTexture::DirectDrawTexture(std::string path)
{
    std::fstream fp(path);
    if (!fp.is_open()) {
#ifdef RENDERER_DEBUG
        std::cerr << "Failed to open file: " << path << std::endl;
#endif
        m_ready = false;
        return;
    }

    size_t len;
    fp.seekg(0L, fp.end);
    len = fp.tellg();
    fp.seekg(0L, fp.beg);

    char* buff = new char[len];
    memset(buff, 0, len);

    fp.read(buff, len);
    fp.close();

    if (!this->load(buff, len)) {
#ifdef RENDERER_DEBUG
        std::cerr << "Failed to load DDS Texture." << std::endl;
#endif
        m_ready = false;
        return;
    }

    m_ready = true;
}

bool DirectDrawTexture::load(char* buff, size_t len)
{
    char header[124] = {0};
    char filecode[4] = {0};

    if (len < 128) {
#ifdef RENDERER_DEBUG
        std::cerr << "Invalid DDSTexture size." << std::endl;
#endif
        return false;
    }

    memcpy(filecode, buff, 4);
    memcpy(header, buff + 4, 124);

    m_buffer.resize(len - 128);
    memcpy(m_buffer.data(), buff + 128, len - 128);

    if (strncmp(filecode, "DDS ", 4) != 0) {
#ifdef RENDERER_DEBUG
        std::cerr << "Invalid filecode for DDS Texture." << std::endl;
#endif
        return false;
    }

    m_height =     *(uint32_t*)&(header[ 8]);
    m_width =      *(uint32_t*)&(header[12]);
    m_linearsize = *(uint32_t*)&(header[16]);
    m_mmcount =    *(uint32_t*)&(header[24]);
    m_fourcc =     *(uint32_t*)&(header[80]);

    switch (m_fourcc) {
    case DXT1:
        m_level = DXT1;
        break;
    case DXT3:
        m_level = DXT3;
        break;
    case DXT5:
        m_level = DXT5;
        break;
    default:
#ifdef RENDERER_DEBUG
        std::cerr << "Undefined compression type for DDS Texture.";
        std::cerr << std::endl;
#endif
        return false;
    }

    return true;
}

bool
DirectDrawTexture::isValid(void)
{
    return m_ready;
}

std::string
DirectDrawTexture::getTextureType(void)
{
    switch (m_level) {
    case DXT1:
        return "DirectDrawSurface_DXT1";
    case DXT3:
        return "DirectDrawSurface_DXT3";
    case DXT5:
        return "DirectDrawSurface_DXT5";
    }
}

uint32_t
DirectDrawTexture::getMipMapCount(void)
{
    return m_mmcount;
}

uint32_t
DirectDrawTexture::getWidth(void)
{
    return m_width;
}

uint32_t
DirectDrawTexture::getHeight(void)
{
    return m_height;
}

std::vector<char>
DirectDrawTexture::getPixelData(void)
{
    std::vector<char> ret;
    ret.resize(m_buffer.size());
    memcpy(ret.data(), m_buffer.data(), m_buffer.size());

    return ret;
}
