#include <gltexture.h>

std::shared_ptr<GLTexture> GLTexture::Create(std::shared_ptr<Texture> tex)
{
    std::shared_ptr<GLTexture> ret(new GLTexture());

    glGenTextures(1, &ret->m_id);
    glBindTexture(GL_TEXTURE_2D, ret->m_id);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    size_t bsize;
    GLenum format;
    std::string strformat = tex->getTextureType();

    if (strformat.compare("DirectDrawSurface_DXT1") == 0) {
        format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        bsize = 8;
    } else if (strformat.compare("DirectDrawSurface_DXT3") == 0) {
        format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        bsize = 16;
    } else if (strformat.compare("DirectDrawSurface_DXT5") == 0) {
        format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        bsize = 16;
    } else {
#ifdef RENDERER_DEBUG
        std::cerr << "Unrecognized image format: " << strformat << std::endl;
#endif
        return nullptr;
    }

    uint32_t offset = 0;
    uint32_t mmc = tex->getMipMapCount();
    uint32_t width = tex->getWidth();
    uint32_t height = tex->getHeight();

    for (uint32_t level = 0; level < mmc && (width || height); level++) {
        size_t size = ((width + 3) / 4) * ((height + 3) / 4) * bsize;
        glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
            0, size, tex->getPixelData().data() + offset);

        offset += size;
        width /= 2;
        height /= 2;

        if (width < 1) {
            width = 1;
        }

        if (height < 1) {
            height = 1;
        }
    }

    ret->m_valid = true;
    return ret;
}

void GLTexture::Destroy(std::shared_ptr<GLTexture> gltex)
{
    glDeleteTextures(1, &gltex->m_id);
    gltex->m_valid = false;
}

bool GLTexture::isValid(void)
{
    return m_valid;
}

GLuint GLTexture::getID(void)
{
    if (!isValid()) {
        return 0;
    }

    return m_id;
}
