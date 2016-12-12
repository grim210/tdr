#ifndef RENDERER_CAMERA_H
#define RENDERER_CAMERA_H

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <renderobject.h>

class Camera {
public:
    Camera(void) { };
    virtual ~Camera(void);
    static std::unique_ptr<Camera> Create(void);
    glm::mat4 getView(void);
private:
    glm::vec3 m_eye;
    glm::vec3 m_up;
    glm::vec3 m_focus;

    std::shared_ptr<RenderObject> m_focusobject;
};

#endif
