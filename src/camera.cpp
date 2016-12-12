#include <camera.h>

Camera::~Camera(void)
{

}

std::unique_ptr<Camera> Camera::Create(void)
{
    std::unique_ptr<Camera> ret(new Camera());

    /* location of camera */
    ret->m_eye[0] = 4.0f;
    ret->m_eye[1] = 3.0f;
    ret->m_eye[2] = -3.0f;

    /* Y-axis is up */
    ret->m_up[0] = 0.0f;
    ret->m_up[1] = 1.0f;
    ret->m_up[2] = 0.0f;

    /* focus on (0, 0, 0) to start */
    ret->m_focus[0] = 0.0f;
    ret->m_focus[1] = 0.0f;
    ret->m_focus[2] = 0.0f;

    return ret;
}

glm::mat4 Camera::getView(void)
{
    if (m_focusobject) {
        m_focus = m_focusobject->getPosition();
    }

    return glm::lookAt(m_eye, m_focus, m_up);
}
