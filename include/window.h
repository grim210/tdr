#ifndef WINDOW_H
#define WINDOW_H

#include <memory>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define DEFAULT_WINDOW_TITLE    ("Renderer")

class Window {
public:
    Window() { };
    virtual ~Window() { };
    static void Destroy(Window* win);
    static std::unique_ptr<Window> Initialize(int w, int h, bool fs);
    void clear(void);
    bool swap(void);

    void setClearColor(float r, float g, float b, float a);
private:
    int m_width, m_height;
    GLFWwindow* m_window;
};

#endif /* WINDOW_H */
