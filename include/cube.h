#ifndef RENDERER_CUBE_H
#define RENDERER_CUBE_H

#ifdef RENDERER_DEBUG
#include <iostream>
#endif

#include <memory>
#include <string>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <gltexture.h>
#include <renderobject.h>
#include <shaderprogram.h>
#include <texture.h>
#include <util.h>

class Cube : public RenderObject {
public:
    Cube(void) { }
    virtual ~Cube(void) { }
    static std::unique_ptr<Cube> Create(std::shared_ptr<Texture> tex);
    static void Destroy(Cube* tri);
    void draw(void);
    glm::vec3 getPosition(void);
    void update(double elapsed, glm::mat4 view, glm::mat4 proj);
private:
    std::unique_ptr<ShaderProgram> m_program;
    //std::unique_ptr<DirectDrawTexture> m_texture;
    std::shared_ptr<GLTexture> m_texture;

    GLint m_color_attrib;
    GLint m_uv_attrib;
    GLint m_vpos_attrib;

    GLint m_mvp_uniform;
    GLint m_tex_uniform;

    GLuint m_uvbuff;
    GLuint m_vbuff;
    GLuint m_tex;

    glm::mat4 m_proj;
    glm::mat4 m_view;
    glm::mat4 m_model;
    glm::mat4 m_mvp;
};

#endif /* RENDERER_CUBE_H */
