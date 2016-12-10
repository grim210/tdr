#include "programmanager.h"

std::unique_ptr<ProgramManager>
ProgramManager::Create(void)
{
    std::unique_ptr<ProgramManager> pm(new ProgramManager());
    pm->m_linked = false;
    pm->m_id = glCreateProgram();

    if (pm->m_id == 0) {
        return nullptr;
    }

    return pm;
}

void
ProgramManager::Destroy(ProgramManager* pm)
{
    pm->m_linked = false;

    for (size_t i = 0; i < pm->m_shaders.size(); i++) {
        glDeleteShader(pm->m_shaders[i].id);
    }

    glDeleteProgram(pm->m_id);
}

bool
ProgramManager::attachShader(GLenum type, std::string source)
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
