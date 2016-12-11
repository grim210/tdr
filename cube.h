#ifndef RENDERER_CUBE_H
#define RENDERER_CUBE_H

#ifdef RENDERER_DEBUG
#include <iostream>
#endif

#include <memory>
#include <string>
#include <glad/glad.h>
#include "linmath.h"
#include "shaderprogram.h"
#include "util.h"

class Cube {
public:
    Cube(void) { }
    virtual ~Cube(void) { }
    static std::unique_ptr<Cube> Create(void);
    static void Destroy(Cube* tri);
    void draw(void);
    void update(double elapsed);
private:
    std::unique_ptr<ShaderProgram> m_program;
    std::unique_ptr<util::DDSTexture> m_texture;

    GLint m_color_attrib;
    GLint m_uv_attrib;
    GLint m_vpos_attrib;

    GLint m_mvp_uniform;
    GLint m_tex_uniform;

    GLuint m_uvbuff;
    GLuint m_vbuff;
    GLuint m_tex;

    mat4x4 m_proj;
    mat4x4 m_view;
    mat4x4 m_model;
    mat4x4 m_mvp;
};

#endif /* RENDERER_CUBE_H */
