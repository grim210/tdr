#include <fstream>
#include <iostream>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <camera.h>
#include <directdrawtexture.h>
#include <renderobject.h>
#include <window.h>

class Model : public RenderObject {
public:
    Model(void) { };
    virtual ~Model(void) { };
    static std::shared_ptr<Model> Create(void);
    static void Delete(std::shared_ptr<Model> model);
private:
    std::vector<float> m_vertices;
    std::vector<float> m_uvs;
};

int main(int argc, char* argv[])
{
    std::unique_ptr<Window> window = Window::Initialize(800, 600, false);
    if (window == nullptr) {
        std::cerr << "nullptr returned.  Aborting." << std::endl;
        return -1;
    }
    window->setClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    std::shared_ptr<DirectDrawTexture> ddtex(
        new DirectDrawTexture("textures/uvtemplate.dds"));
    //std::shared_ptr<Cube> cube = Cube::Create(ddtex);
    std::shared_ptr<Camera> camera = Camera::Create(cube);

    glm::mat4 proj = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);

    bool running = true;
    while (running) {
        double elapsed = glfwGetTime();

        //cube->update(elapsed, camera->getView(), proj);
        window->clear();
        //cube->draw();
        running = window->swap();
    }

    //Cube::Destroy(cube.get());
    Window::Destroy(window.get());
    return 0;
}
