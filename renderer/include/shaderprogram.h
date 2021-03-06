#ifndef PROGRAMMANAGER_H
#define PROGRAMMANAGER_H

#ifdef TDR_DEBUG
#include <iostream>
#endif

#include <memory>
#include <string>
#include <vector>

#include <glad/glad.h>

class ShaderProgram {
public:
    static std::shared_ptr<ShaderProgram> Create(void);
    static void Destroy(std::shared_ptr<ShaderProgram> pm);
    bool attachShader(GLenum type, std::string source);
    GLuint getProgram(void);
    GLint getAttributeLocation(std::string name);
    GLint getUniformLocation(std::string name);
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
