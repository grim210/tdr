#ifndef PROGRAMMANAGER_H
#define PROGRAMMANAGER_H

#ifdef RENDERER_DEBUG
#include <iostream>
#endif

#include <memory>
#include <string>
#include <vector>

#include <glad/glad.h>

class ShaderProgram {
public:
    static std::unique_ptr<ShaderProgram> Create(void);
    static void Destroy(ShaderProgram* pm);
    bool attachShader(GLenum type, std::string source);
    GLuint getProgram(void);
    bool link(void);
private:
    struct Shader {
        bool compiled;
        GLuint id;
        GLenum type;
        std::string source;
    };

    GLuint m_id;
    bool m_linked;
    std::vector<Shader> m_shaders;
};

#endif /* PROGRAMMANAGER_H */
