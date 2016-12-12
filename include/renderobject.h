#ifndef RENDERER_RENDEROBJECT_H
#define RENDERER_RENDEROBJECT_H

#include <glm/glm.hpp>

class RenderObject {
public:
    virtual void draw(void) = 0;
    virtual glm::vec3 getPosition(void) = 0;
    virtual void update(double elapsed, glm::mat4 view, glm::mat4 proj) = 0;
};

#endif /* RENDERER_RENDEROBJECT_H */
