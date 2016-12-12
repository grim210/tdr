#include <cube.h>

std::unique_ptr<Cube> Cube::Create(std::shared_ptr<Texture> tex)
{
    std::unique_ptr<Cube> box(new Cube());
    box->m_program = ShaderProgram::Create();

    std::string vshader = util::load_text_file("./shaders/vshader.vs");
    std::string fshader = util::load_text_file("./shaders/fshader.fs");

    if (!box->m_program->attachShader(GL_VERTEX_SHADER, vshader)) {
#ifdef RENDERER_DEBUG
        std::cerr << "Failed to attach vertex shader! ::" << std::endl;
        std::cerr << vshader << std::endl;
#endif
        return nullptr;
    }

    if (!box->m_program->attachShader(GL_FRAGMENT_SHADER, fshader)) {
#ifdef RENDERER_DEBUG
        std::cerr << "Failed to attach fragment shader! ::" << std::endl;
        std::cerr << fshader << std::endl;
#endif
        return nullptr;
    }

    if (!box->m_program->link()) {
#ifdef RENDERER_DEBUG
        std::cerr << "Failed to link shader program." << std::endl;
#endif
        return nullptr;
    }

    box->m_uv_attrib = glGetAttribLocation(box->m_program->getProgram(),
        "vertexUV");
    if (box->m_uv_attrib < 0) {
#ifdef RENDERER_DEBUG
        std::cerr << "Failed to find UV attribute location." << std::endl;
#endif
        return nullptr;
    }

    box->m_vpos_attrib = glGetAttribLocation(box->m_program->getProgram(),
        "vertexPosition_modelspace");
    if (box->m_vpos_attrib < 0) {
#ifdef RENDERER_DEBUG
        std::cerr << "Failed to find vertex attribute location." << std::endl;
#endif
        return nullptr;
    }

    box->m_mvp_uniform = glGetUniformLocation(box->m_program->getProgram(),
        "MVP");
    if (box->m_mvp_uniform < 0) {
#ifdef RENDERER_DEBUG
        std::cerr << "Failed to find the MVP uniform." << std::endl;
#endif
        return nullptr;
    }

    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    static const GLfloat g_uv_buffer_data[] = {
        0.000059f, 1.0f-0.000004f,
        0.000103f, 1.0f-0.336048f,
        0.335973f, 1.0f-0.335903f,
        1.000023f, 1.0f-0.000013f,
        0.667979f, 1.0f-0.335851f,
        0.999958f, 1.0f-0.336064f,
        0.667979f, 1.0f-0.335851f,
        0.336024f, 1.0f-0.671877f,
        0.667969f, 1.0f-0.671889f,
        1.000023f, 1.0f-0.000013f,
        0.668104f, 1.0f-0.000013f,
        0.667979f, 1.0f-0.335851f,
        0.000059f, 1.0f-0.000004f,
        0.335973f, 1.0f-0.335903f,
        0.336098f, 1.0f-0.000071f,
        0.667979f, 1.0f-0.335851f,
        0.335973f, 1.0f-0.335903f,
        0.336024f, 1.0f-0.671877f,
        1.000004f, 1.0f-0.671847f,
        0.999958f, 1.0f-0.336064f,
        0.667979f, 1.0f-0.335851f,
        0.668104f, 1.0f-0.000013f,
        0.335973f, 1.0f-0.335903f,
        0.667979f, 1.0f-0.335851f,
        0.335973f, 1.0f-0.335903f,
        0.668104f, 1.0f-0.000013f,
        0.336098f, 1.0f-0.000071f,
        0.000103f, 1.0f-0.336048f,
        0.000004f, 1.0f-0.671870f,
        0.336024f, 1.0f-0.671877f,
        0.000103f, 1.0f-0.336048f,
        0.336024f, 1.0f-0.671877f,
        0.335973f, 1.0f-0.335903f,
        0.667969f, 1.0f-0.671889f,
        1.000004f, 1.0f-0.671847f,
        0.667979f, 1.0f-0.335851f
    };

    box->m_texture = std::shared_ptr<GLTexture>(GLTexture::Create(tex));
    if (!box->m_texture->isValid()) {
#ifdef RENDERER_DEBUG
        std::cerr << "Failed to create OpenGL texture." << std::endl;
#endif
        return nullptr;
    }

    glGenBuffers(1, &box->m_vbuff);
    glBindBuffer(GL_ARRAY_BUFFER, box->m_vbuff);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
        g_vertex_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &box->m_uvbuff);
    glBindBuffer(GL_ARRAY_BUFFER, box->m_uvbuff);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data),
        g_uv_buffer_data, GL_STATIC_DRAW);

    glm::vec3 eye(4.0f, 3.0f, -3.0f);
    glm::vec3 center(0.0f, 0.0f, 0.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);

    box->m_view = glm::lookAt(eye, center, up);
    box->m_proj = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
    box->m_model = glm::mat4(1.0f);
    box->m_mvp = box->m_proj * box->m_view * box->m_model;

    return box;
}

void
Cube::Destroy(Cube* box)
{
    glDeleteBuffers(1, &box->m_vbuff);
    ShaderProgram::Destroy(box->m_program.get());
}

void
Cube::draw(void)
{
    glUseProgram(m_program->getProgram());
    glUniformMatrix4fv(m_mvp_uniform, 1, GL_FALSE, glm::value_ptr(m_mvp));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture->getID());
    glUniform1i(m_tex_uniform, 0);

    glEnableVertexAttribArray(m_vpos_attrib);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbuff);
    glVertexAttribPointer(m_vpos_attrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(m_uv_attrib);
    glBindBuffer(GL_ARRAY_BUFFER, m_uvbuff);
    glVertexAttribPointer(m_uv_attrib, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glDrawArrays(GL_TRIANGLES, 0, 12*3);
    glDisableVertexAttribArray(m_vpos_attrib);
    glDisableVertexAttribArray(m_uv_attrib);
}

glm::vec3 Cube::getPosition(void)
{
    /*
    std::shared_ptr<vec3> pos = make_shared<vec3>();
    pos[0] = 0.0f;
    pos[1] = 0.0f;
    pos[2] = 0.0f;
    */
    return glm::vec3(0.0f, 0.0f, 0.0f);
}

void
Cube::update(double elapsed, glm::mat4 view, glm::mat4 proj)
{

}
