#ifndef RENDERER_GLTEXTURE_H
#define RENDERER_GLTEXTURE_H

#ifdef RENDERER_DEBUG
#include <iostream>
#endif

#include <memory>
#include <glad/glad.h>
#include <texture.h>

class GLTexture {
public:
    GLTexture(void) { };
    virtual ~GLTexture(void) { };
    static std::shared_ptr<GLTexture> Create(std::shared_ptr<Texture> tex);
    static void Destroy(std::shared_ptr<GLTexture> gltex);
    bool isValid(void);
    GLuint getID(void);
private:
    bool m_valid;
    GLuint m_id;
};

extern "C" {
    const char* get_renderer_version(void);
}

#endif /* RENDERER_GLTEXTURE_H */
