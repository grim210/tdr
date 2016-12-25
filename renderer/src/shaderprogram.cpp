#include <shaderprogram.h>

std::shared_ptr<ShaderProgram> ShaderProgram::Create(void)
{
    std::unique_ptr<ShaderProgram> pm(new ShaderProgram());

    pm->m_linked = false;
    pm->m_id = glCreateProgram();

    if (pm->m_id == 0) {
        return nullptr;
    }

    return pm;
}

void ShaderProgram::Destroy(std::shared_ptr<ShaderProgram> pm)
{
    pm->m_linked = false;

    for (size_t i = 0; i < pm->m_shaders.size(); i++) {
        glDeleteShader(pm->m_shaders[i].id);
    }

    glDeleteProgram(pm->m_id);
}

bool ShaderProgram::attachShader(GLenum type, std::string source)
{
    if (source.length() < 1) {
        return false;
    }

    Shader sh = {0};
    sh.compiled = false;
    sh.type = type;
    sh.source = std::string(source);
    sh.id = glCreateShader(type);

    if (sh.id == 0) {
        return false;
    }

    const char* src = sh.source.c_str();
    glShaderSource(sh.id, 1, &src, nullptr);
    glCompileShader(sh.id);

    GLint result = 0;
    glGetShaderiv(sh.id, GL_COMPILE_STATUS, &result);
    if (result != GL_TRUE) {
        return false;
    }

    m_shaders.push_back(sh);
    return true;
}

GLuint ShaderProgram::getProgram(void)
{
    bool result;

    if (!m_linked) {
        result = this->link();
        if (!result) {
            return 0;
        }
    }

    return m_id;
}

GLint ShaderProgram::getAttributeLocation(std::string name)
{
    GLint result;

    result = glGetAttribLocation(m_id, name.c_str());
    if (result < 0) {
#ifdef TDR_DEBUG
        std::cerr << "Failed to find attribute '" << name << "'." << std::endl;
#endif
        return -1;
    }

    return result;
}

GLint ShaderProgram::getUniformLocation(std::string name)
{
    GLint result = glGetUniformLocation(m_id, name.c_str());
    if (result < 0) {
#ifdef TDR_DEBUG
        std::cerr << "Failed to find uniform '" << name << "'." << std::endl;
#endif
        return -1;
    }

    return result;
}

bool ShaderProgram::link(void)
{
    GLint result;

    if (m_linked) {
        return true;
    }

    for (size_t i = 0; i < m_shaders.size(); i++) {
        glAttachShader(m_id, m_shaders[i].id);
    }

    glLinkProgram(m_id);
    glGetProgramiv(m_id, GL_LINK_STATUS, &result);
    if (result == GL_FALSE) {
        m_linked = false;
    } else {
        m_linked = true;
    }

    return m_linked;
}
