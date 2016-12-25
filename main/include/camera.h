#ifndef RENDERER_CAMERA_H
#define RENDERER_CAMERA_H

#ifdef TDRMAIN_DEBUG
#include <iostream>
#endif

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <renderobject.h>

class Camera {
public:
    Camera(void) { };
    virtual ~Camera(void);
    static std::unique_ptr<Camera> Create(void);
    static std::unique_ptr<Camera> Create(glm::vec3 focus);
    static std::unique_ptr<Camera> Create(std::shared_ptr<RenderObject> obj);
    glm::mat4 getView(void);
    bool setFocus(std::shared_ptr<RenderObject> obj);
private:
    glm::vec3 m_eye;
    glm::vec3 m_up;
    glm::vec3 m_focus;
    std::shared_ptr<RenderObject> m_focusobject;
};

#endif
