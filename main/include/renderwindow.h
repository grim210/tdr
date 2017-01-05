#ifndef TDR_RENDERWINDOW_H
#define TDR_RENDERWINDOW_H

class RenderWindow {
public:
    virtual void clear(void) = 0;
    virtual void setClearColor(float r, float g, float b, float a) = 0;
    virtual void swap(void) = 0;
};

#endif /* TDR_RENDERWINDOW_H */
