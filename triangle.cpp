#include "triangle.h"

std::unique_ptr<Triangle>
Triangle::Create(void)
{
    std::unique_ptr<Triangle> tri(new Triangle());
    tri->m_program = ProgramManager::Create();

    std::string vshader = util::load_text_file("./shaders/vshader.vs");
    std::string fshader = util::load_text_file("./shaders/fshader.fs");

    if (!tri->m_program->attachShader(GL_VERTEX_SHADER, vshader)) {
#ifdef RENDERER_DEBUG
        std::cerr << "Failed to attach vertex shader! ::" << std::endl;
        std::cerr << vshader << std::endl;
#endif
        return nullptr;
    }

    if (!tri->m_program->attachShader(GL_FRAGMENT_SHADER, fshader)) {
#ifdef RENDERER_DEBUG
        std::cerr << "Failed to attach fragment shader! ::" << std::endl;
        std::cerr << fshader << std::endl;
#endif
        return nullptr;
    }

    if (!tri->m_program->link()) {
#ifdef RENDERER_DEBUG
        std::cerr << "Failed to link shader program." << std::endl;
#endif
        return nullptr;
    }

    tri->m_vpos_attrib = glGetAttribLocation(tri->m_program->getProgram(),
        "vertexPosition_modelspace");
    if (tri->m_vpos_attrib < 0) {
#ifdef RENDERER_DEBUG
        std::cerr << "Failed to find vertex attribute location." << std::endl;
#endif
        return nullptr;
    }

    tri->m_mvp_uniform = glGetUniformLocation(tri->m_program->getProgram(),
        "MVP");
    if (tri->m_mvp_uniform < 0) {
#ifdef RENDERER_DEBUG
        std::cerr << "Failed to find the MVP uniform." << std::endl;
#endif
        return nullptr;
    }

    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f,  0.0f,
         1.0f, -1.0f,  0.0f,
         0.0f,  1.0f,  0.0f
    };

    glGenBuffers(1, &tri->m_vbuff);
    glBindBuffer(GL_ARRAY_BUFFER, tri->m_vbuff);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
        g_vertex_buffer_data, GL_STATIC_DRAW);

    vec3 eye = { 4.0f, 3.0f, 3.0f };
    vec3 center = { 0.0f, 0.0f, 0.0f };
    vec3 up = { 0.0f, 1.0f, 0.0f };
    mat4x4_look_at(tri->m_view, eye, center, up);
    mat4x4_perspective(tri->m_proj, 45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
    mat4x4_identity(tri->m_model);

    mat4x4 accum;
    mat4x4_mul(accum, tri->m_view, tri->m_model);
    mat4x4_mul(tri->m_mvp, tri->m_proj, accum);

    return tri;
}

void
Triangle::Destroy(Triangle* tri)
{
    glDeleteBuffers(1, &tri->m_vbuff);
    ProgramManager::Destroy(tri->m_program.get());
}

void
Triangle::draw(void)
{
    glUseProgram(m_program->getProgram());
    glUniformMatrix4fv(m_mvp_uniform, 1, GL_FALSE, (GLfloat*)m_mvp);

    glEnableVertexAttribArray(m_vpos_attrib);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbuff);
    glVertexAttribPointer(m_vpos_attrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(m_vpos_attrib);
}

void
Triangle::update(double elapsed)
{

}
