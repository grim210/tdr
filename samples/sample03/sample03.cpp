#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SDL2/SDL.h>

#include <tdrmain.h>
#include <tdrrenderer.h>

class Model : public RenderObject {
public:
    Model(void) { };
    virtual ~Model(void) { };
    static std::shared_ptr<Model> Create(void);
    static void Delete(std::shared_ptr<Model> model);
    bool addTexture(std::shared_ptr<DirectDrawTexture> ddt);

    /* overridden from RenderObject */
    void draw(void);
    glm::vec3 getPosition(void);
    bool setPosition(glm::vec3 location);
    bool translate(glm::vec3 offset);
    void update(double elapsed, glm::mat4 view, glm::mat4 proj);
private:
    std::shared_ptr<ShaderProgram> m_program;
    std::shared_ptr<GLTexture> m_texture;
    std::shared_ptr<TDRMesh> m_mesh;

    /* raw vertex and uv data */
    std::vector<float> m_vertices;
    std::vector<float> m_uvs;

    GLint m_color_attrib;
    GLint m_uv_attrib;
    GLint m_vpos_attrib;

    GLint m_mvp_uniform;
    GLint m_tex_uniform;

    GLuint m_uvbuffer;
    GLuint m_vbuffer;
    GLuint m_texid;

    glm::vec3 m_pos;
    glm::mat4 m_model;
    glm::mat4 m_mvp;

    void dump_doubles(std::vector<float> arr);
    void dump_triplets(std::vector<float> arr);
};

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVENTS);

    std::unique_ptr<Window> window = Window::Initialize(800, 600, false);
    if (window == nullptr) {
        std::cerr << "nullptr returned.  Aborting." << std::endl;
        return -1;
    }
    window->setClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    std::shared_ptr<Timer> clock = Timer::Create();
    std::shared_ptr<DirectDrawTexture> ddtex(
        new DirectDrawTexture("textures/uvtemplate.dds"));
    std::shared_ptr<Model> model = Model::Create();
    model->addTexture(ddtex);
    std::shared_ptr<Camera> camera = Camera::Create(model);

    glm::mat4 proj = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);

    bool running = true;
    while (running) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) {
                running = false;
            }
        }

        double elapsed = clock->getTime();

        model->update(elapsed, camera->getView(), proj);
        window->clear();
        model->draw();
        window->swap();
    }

    Model::Delete(model);
    Window::Destroy(window.get());

    SDL_Quit();
    return 0;
}

std::shared_ptr<Model> Model::Create(void)
{
    std::shared_ptr<Model> model(new Model());

    std::string vshader = load_text_file("./shaders/vshader.vs");
    std::string fshader = load_text_file("./shaders/fshader.fs");
    std::string modeldata = load_text_file("./cube.json");

#ifdef TDR_DEBUG
    std::cout << "Vertex Shader Code" << std::endl << vshader << std::endl;
    std::cout << "Fragment Shader Code" << std::endl << fshader << std::endl;
#endif

    model->m_program = ShaderProgram::Create();
    model->m_mesh = TDRMesh::Load(modeldata.c_str(), modeldata.length());

    model->m_program->attachShader(GL_VERTEX_SHADER, vshader);
    model->m_program->attachShader(GL_FRAGMENT_SHADER, fshader);
    if (!model->m_program->link()) {
#ifdef TDR_DEBUG
        std::cerr << "Failed to link shader code.  Aborting." << std::endl;
#endif
        return nullptr;
    }

    model->m_uv_attrib = model->m_program->getAttributeLocation("vertexUV");
    model->m_vpos_attrib = model->m_program->getAttributeLocation(
        "vertexPosition_modelspace");
    model->m_mvp_uniform = model->m_program->getUniformLocation("MVP");

    if (model->m_uv_attrib == -1 || model->m_vpos_attrib == -1 ||
        model->m_mvp_uniform == -1) {
#ifdef TDR_DEBUG
        if (model->m_uv_attrib == -1) {
            std::cerr << "Failed to find UV attribute." << std::endl;
        }

        if (model->m_vpos_attrib == -1) {
            std::cerr << "Failed to find vertex position attribute.";
            std::cerr << std::endl;
        }

        if (model->m_mvp_uniform == -1) {
            std::cerr << "Failed to find MVP matrix uniform." << std::endl;
        }
#endif
        return nullptr;
    }

    std::vector<float> verts = model->m_mesh->get(TDRMesh::Vertex);
    std::vector<float> uvs = model->m_mesh->get(TDRMesh::UV);

#ifdef TDR_DEBUG
    model->dump_triplets(verts);
    model->dump_doubles(uvs);
#endif

    glGenBuffers(1, &model->m_vbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, model->m_vbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verts.size(), verts.data(),
        GL_STATIC_DRAW);

    glGenBuffers(1, &model->m_uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, model->m_uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * uvs.size(), uvs.data(),
        GL_STATIC_DRAW);

    model->m_pos = glm::vec3(0.0f, 0.0f, 0.0f);
    model->m_model = glm::translate(glm::mat4(1.0f), model->m_pos);

    return model;
}

void Model::Delete(std::shared_ptr<Model> model)
{
    glDeleteBuffers(1, &model->m_vbuffer);
    glDeleteBuffers(1, &model->m_uvbuffer);
    ShaderProgram::Destroy(model->m_program);
}

bool Model::addTexture(std::shared_ptr<DirectDrawTexture> ddt)
{
    m_texture = std::shared_ptr<GLTexture>(GLTexture::Create(ddt));
    if (!m_texture->isValid()) {
        return false;
    }

    return true;
}

void Model::draw(void)
{
    glUseProgram(m_program->getProgram());
    glUniformMatrix4fv(m_mvp_uniform, 1, GL_FALSE, glm::value_ptr(m_mvp));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture->getID());
    glUniform1i(m_tex_uniform, 0);

    glEnableVertexAttribArray(m_vpos_attrib);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbuffer);
    glVertexAttribPointer(m_vpos_attrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(m_uv_attrib);
    glBindBuffer(GL_ARRAY_BUFFER, m_uvbuffer);
    glVertexAttribPointer(m_uv_attrib, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glDrawArrays(GL_TRIANGLES, 0, 12*3);
    glDisableVertexAttribArray(m_vpos_attrib);
    glDisableVertexAttribArray(m_uv_attrib);
}

glm::vec3 Model::getPosition(void)
{
    return glm::vec3(0.0f, 0.0f, 0.0f);
}

bool Model::setPosition(glm::vec3 position)
{
    m_model = glm::translate(glm::mat4(1.0f), position);
    return true;
}

bool Model::translate(glm::vec3 offset)
{
    m_model = glm::translate(m_model, offset);
    return true;
}

void Model::update(double elapsed, glm::mat4 view, glm::mat4 proj)
{
    m_mvp = proj * view * m_model;
}

void Model::dump_triplets(std::vector<float> arr)
{
    for (size_t i = 0; i < arr.size(); i++) {
        if ((i != 0) && (i % 3 == 0)) {
            std::cout << std::endl;
        }
    
        fprintf(stdout, "%.4f, ", arr[i]);
    }
    std::cout << std::endl;
}

void Model::dump_doubles(std::vector<float> arr)
{
    for (size_t i = 0; i < arr.size(); i++) {
        if ((i != 0) && (i % 2 == 0)) {
            std::cout << std::endl;
        }

        fprintf(stdout, "%.4f, ", arr[i]);
    }
    std::cout << std::endl;
}
