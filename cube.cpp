#include "cube.h"

std::unique_ptr<Cube>
Cube::Create(void)
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

    box->m_color_attrib = glGetAttribLocation(box->m_program->getProgram(),
        "vertexColor");
    if (box->m_color_attrib < 0) {
#ifdef RENDERER_DEBUG
        std::cerr << "Failed to find color attribute location." << std::endl;
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

    static const GLfloat g_color_buffer_data[] = {
        0.583f,  0.771f,  0.014f,
        0.609f,  0.115f,  0.436f,
        0.327f,  0.483f,  0.844f,
        0.822f,  0.569f,  0.201f,
        0.435f,  0.602f,  0.223f,
        0.310f,  0.747f,  0.185f,
        0.597f,  0.770f,  0.761f,
        0.559f,  0.436f,  0.730f,
        0.359f,  0.583f,  0.152f,
        0.483f,  0.596f,  0.789f,
        0.559f,  0.861f,  0.639f,
        0.195f,  0.548f,  0.859f,
        0.014f,  0.184f,  0.576f,
        0.771f,  0.328f,  0.970f,
        0.406f,  0.615f,  0.116f,
        0.676f,  0.977f,  0.133f,
        0.971f,  0.572f,  0.833f,
        0.140f,  0.616f,  0.489f,
        0.997f,  0.513f,  0.064f,
        0.945f,  0.719f,  0.592f,
        0.543f,  0.021f,  0.978f,
        0.279f,  0.317f,  0.505f,
        0.167f,  0.620f,  0.077f,
        0.347f,  0.857f,  0.137f,
        0.055f,  0.953f,  0.042f,
        0.714f,  0.505f,  0.345f,
        0.783f,  0.290f,  0.734f,
        0.722f,  0.645f,  0.174f,
        0.302f,  0.455f,  0.848f,
        0.225f,  0.587f,  0.040f,
        0.517f,  0.713f,  0.338f,
        0.053f,  0.959f,  0.120f,
        0.393f,  0.621f,  0.362f,
        0.673f,  0.211f,  0.457f,
        0.820f,  0.883f,  0.371f,
        0.982f,  0.099f,  0.879f
    };

    glGenBuffers(1, &box->m_vbuff);
    glBindBuffer(GL_ARRAY_BUFFER, box->m_vbuff);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
        g_vertex_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &box->m_cbuff);
    glBindBuffer(GL_ARRAY_BUFFER, box->m_cbuff);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data),
        g_color_buffer_data, GL_STATIC_DRAW);

    vec3 eye = { 4.0f, 3.0f, -3.0f };
    vec3 center = { 0.0f, 0.0f, 0.0f };
    vec3 up = { 0.0f, 1.0f, 0.0f };
    mat4x4_look_at(box->m_view, eye, center, up);
    mat4x4_perspective(box->m_proj, 45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
    mat4x4_identity(box->m_model);

    mat4x4 accum;
    mat4x4_mul(accum, box->m_view, box->m_model);
    mat4x4_mul(box->m_mvp, box->m_proj, accum);

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
    glUniformMatrix4fv(m_mvp_uniform, 1, GL_FALSE, (GLfloat*)m_mvp);

    glEnableVertexAttribArray(m_vpos_attrib);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbuff);
    glVertexAttribPointer(m_vpos_attrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(m_color_attrib);
    glBindBuffer(GL_ARRAY_BUFFER, m_cbuff);
    glVertexAttribPointer(m_color_attrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glDrawArrays(GL_TRIANGLES, 0, 12*3);
    glDisableVertexAttribArray(m_vpos_attrib);
    glDisableVertexAttribArray(m_color_attrib);
}

void
Cube::update(double elapsed)
{

}
