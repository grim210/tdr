#include <fstream>
#include <iostream>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <tdrmain.h>
#include <tdrrenderer.h>

class Cube : public RenderObject {
public:
    Cube(void) { }
    virtual ~Cube(void) { }
    static std::shared_ptr<Cube> Create(std::shared_ptr<Texture> tex);
    static void Destroy(Cube* tri);
    void draw(void);
    glm::vec3 getPosition(void);
    bool setPosition(glm::vec3 location);
    bool translate(glm::vec3 destination);
    void update(double elapsed, glm::mat4 view, glm::mat4 proj);
private:
    std::shared_ptr<ShaderProgram> m_program;
    std::shared_ptr<GLTexture> m_texture;
    std::shared_ptr<TDRMesh> m_mesh;

    GLint m_color_attrib;
    GLint m_uv_attrib;
    GLint m_vpos_attrib;

    GLint m_mvp_uniform;
    GLint m_tex_uniform;

    GLuint m_uvbuff;
    GLuint m_vbuff;
    GLuint m_tex;

    glm::vec3 m_pos;
    glm::mat4 m_model;
    glm::mat4 m_mvp;
};

int main(int argc, char* argv[])
{
    std::unique_ptr<Window> window = Window::Initialize(800, 600, false);
    if (window == nullptr) {
        std::cerr << "nullptr returned.  Aborting." << std::endl;
        return -1;
    }
    window->setClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    std::shared_ptr<Timer> clock = Timer::Create();
    std::shared_ptr<DirectDrawTexture> ddtex(
        new DirectDrawTexture("textures/uvtemplate.dds"));
    std::shared_ptr<Cube> cube = Cube::Create(ddtex);
    std::shared_ptr<Camera> camera = Camera::Create(cube);

    glm::mat4 proj = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);

    clock->start();

    bool running = true;
    while (running) {
        double elapsed = clock->getTime();

        cube->update(elapsed, camera->getView(), proj);
        window->clear();
        cube->draw();
        window->swap();
    }

    Cube::Destroy(cube.get());
    Window::Destroy(window.get());
    return 0;
}

std::shared_ptr<Cube> Cube::Create(std::shared_ptr<Texture> tex)
{
    std::shared_ptr<Cube> box(new Cube());
    box->m_program = ShaderProgram::Create();

    std::string vshader = load_text_file("./shaders/vshader.vs");
    std::string fshader = load_text_file("./shaders/fshader.fs");

    if (!box->m_program->attachShader(GL_VERTEX_SHADER, vshader)) {
#ifdef TDR_DEBUG
        std::cerr << "Failed to attach vertex shader! ::" << std::endl;
        std::cerr << vshader << std::endl;
#endif
        return nullptr;
    }

    if (!box->m_program->attachShader(GL_FRAGMENT_SHADER, fshader)) {
#ifdef TDR_DEBUG
        std::cerr << "Failed to attach fragment shader! ::" << std::endl;
        std::cerr << fshader << std::endl;
#endif
        return nullptr;
    }

    if (!box->m_program->link()) {
#ifdef TDR_DEBUG
        std::cerr << "Failed to link shader program." << std::endl;
#endif
        return nullptr;
    }

    box->m_uv_attrib = glGetAttribLocation(box->m_program->getProgram(),
        "vertexUV");
    if (box->m_uv_attrib < 0) {
#ifdef TDR_DEBUG
        std::cerr << "Failed to find UV attribute location." << std::endl;
#endif
        return nullptr;
    }

    box->m_vpos_attrib = glGetAttribLocation(box->m_program->getProgram(),
        "vertexPosition_modelspace");
    if (box->m_vpos_attrib < 0) {
#ifdef TDR_DEBUG
        std::cerr << "Failed to find vertex attribute location." << std::endl;
#endif
        return nullptr;
    }

    box->m_mvp_uniform = glGetUniformLocation(box->m_program->getProgram(),
        "MVP");
    if (box->m_mvp_uniform < 0) {
#ifdef TDR_DEBUG
        std::cerr << "Failed to find the MVP uniform." << std::endl;
#endif
        return nullptr;
    }

    std::vector<float> verts = box->m_mesh->get(TDRMesh::Vertex);
    std::vector<float> uvs = box->m_mesh->get(TDRMesh::UV);

    box->m_texture = std::shared_ptr<GLTexture>(GLTexture::Create(tex));
    if (!box->m_texture->isValid()) {
#ifdef TDR_DEBUG
        std::cerr << "Failed to create OpenGL texture." << std::endl;
#endif
        return nullptr;
    }

    glGenBuffers(1, &box->m_vbuff);
    glBindBuffer(GL_ARRAY_BUFFER, box->m_vbuff);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verts.size(), verts.data(),
        GL_STATIC_DRAW);

    glGenBuffers(1, &box->m_uvbuff);
    glBindBuffer(GL_ARRAY_BUFFER, box->m_uvbuff);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * uvs.size(), uvs.data(),
        GL_STATIC_DRAW);

    box->m_pos = glm::vec3(0.0f, 0.0f, 0.0f);
    box->m_model = glm::translate(glm::mat4(1.0f), box->m_pos);

    return box;
}

void Cube::Destroy(Cube* box)
{
    glDeleteBuffers(1, &box->m_vbuff);
    glDeleteBuffers(1, &box->m_uvbuff);
    ShaderProgram::Destroy(box->m_program);
}

void Cube::draw(void)
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
    return glm::vec3(0.0f, 0.0f, 0.0f);
}

bool Cube::setPosition(glm::vec3 destination)
{
    m_model = glm::translate(glm::mat4(1.0f), destination);
    return true;
}

bool Cube::translate(glm::vec3 offset)
{
    m_model = glm::translate(m_model, offset);
    return true;
}

void Cube::update(double elapsed, glm::mat4 view, glm::mat4 proj)
{
    m_mvp = proj * view * m_model;
}
