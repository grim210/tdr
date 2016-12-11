#ifndef RENDERER_RENDEROBJECT_H
#define RENDERER_RENDEROBJECT_H

class RenderObject {
    virtual void draw(void) = 0;
    virtual void update(double elapsed, mat4x4 view, mat4x4 proj) = 0;
};

#endif /* RENDERER_RENDEROBJECT_H */
