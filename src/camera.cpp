#include <camera.h>

Camera::~Camera(void)
{

}

std::unique_ptr<Camera> Camera::Create(void)
{
    std::unique_ptr<Camera> ret(new Camera());

    ret->m_eye = glm::vec3(4.0f, 3.0f, -3.0f);
    ret->m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    ret->m_focus = glm::vec3(0.0f, 0.0f, 0.0f);

    return ret;
}

std::unique_ptr<Camera> Camera::Create(glm::vec3 focus)
{
    std::unique_ptr<Camera> ret(new Camera());

    ret->m_eye = glm::vec3(4.0f, 3.0f, -3.0f);
    ret->m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    ret->m_focus = focus;

    return ret;
}

std::unique_ptr<Camera> Camera::Create(std::shared_ptr<RenderObject> obj)
{
    std::unique_ptr<Camera> ret(new Camera());

    ret->m_eye = glm::vec3(4.0f, 3.0f, -3.0f);
    ret->m_up = glm::vec3(0.0f, 1.0f, 0.0f);

    ret->m_focusobject = obj;
    ret->m_focus = obj->getPosition();
    
    return ret;
}

glm::mat4 Camera::getView(void)
{
    if (m_focusobject) {
        m_focus = m_focusobject->getPosition();
    }

    glm::vec3 eye = glm::vec3(4.0f, 3.0f, -3.0f);
    return glm::lookAt(eye, m_focus, m_up);
}

bool Camera::setFocus(std::shared_ptr<RenderObject> obj)
{
    m_focusobject = obj;
}
