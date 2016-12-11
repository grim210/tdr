#ifndef RENDERER_TRIANGLE
#define RENDERER_TRIANGLE

#ifdef RENDERER_DEBUG
#include <iostream>
#endif

#include <memory>
#include <string>
#include <glad/glad.h>
#include "linmath.h"
#include "programmanager.h"
#include "util.h"

class Triangle {
public:
    Triangle(void) { }
    virtual ~Triangle(void) { }
    static std::unique_ptr<Triangle> Create(void);
    static void Destroy(Triangle* tri);
    void draw(void);
    void update(double elapsed);
private:
    std::unique_ptr<ProgramManager> m_program;
    GLint m_vpos_attrib;
    GLint m_mvp_uniform;
    GLuint m_vbuff;

    mat4x4 m_proj;
    mat4x4 m_view;
    mat4x4 m_model;
    mat4x4 m_mvp;
};

#endif /* RENDERER_TRIANGLE */
