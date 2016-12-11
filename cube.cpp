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

    box->m_texture = std::unique_ptr<DirectDrawTexture>(
        new DirectDrawTexture("textures/uvtemplate.dds"));
    if (!box->m_texture->isValid()) {
#ifdef RENDERER_DEBUG
        std::cerr << "Failed to load texture uvtemplate.dds" << std::endl;
#endif
        return nullptr;
    }

    glGenTextures(1, &box->m_tex);
    glBindTexture(GL_TEXTURE_2D, box->m_tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    GLenum format;

    unsigned int bsize;
    std::string strformat = box->m_texture->getTextureType();
    if (strformat.compare("DirectDrawSurface_DXT1") == 0) {
        format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        bsize = 8;
    } else if (strformat.compare("DirectDrawSurface_DXT3") == 0) {
        format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        bsize = 16;
    } else if (strformat.compare("DirectDrawSurface_DXT5") == 0) {
        format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        bsize = 16;
    } else {
#ifdef RENDERER_DEBUG
        std::cerr << "Unrecognized compression format for DirectDrawTexture.";
        std::cerr << std::endl;
#endif
        return nullptr;
    }

    unsigned int offset = 0;
    unsigned int mmc = box->m_texture->getMipMapCount();
    unsigned int width = box->m_texture->getWidth();
    unsigned int height = box->m_texture->getHeight();

    for (unsigned int level = 0; level < mmc && (width || height); level++) {
        size_t size = ((width + 3) / 4) * ((height + 3) / 4) * bsize;
        glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
            0, size, box->m_texture->getPixelData().data() + offset);

        offset += size;
        width /= 2;
        height /= 2;

        if (width < 1) {
            width = 1;
        }

        if (height < 1) {
            height = 1;
        }
    }

    glGenBuffers(1, &box->m_vbuff);
    glBindBuffer(GL_ARRAY_BUFFER, box->m_vbuff);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
        g_vertex_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &box->m_uvbuff);
    glBindBuffer(GL_ARRAY_BUFFER, box->m_uvbuff);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data),
        g_uv_buffer_data, GL_STATIC_DRAW);

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

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_tex);
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

void
Cube::update(double elapsed, mat4x4 view, mat4x4 proj)
{

}
