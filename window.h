#ifndef WINDOW_H
#define WINDOW_H

#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
public:
    Window() { };
    virtual ~Window() { };
    static void Destroy(Window* win);
    static std::unique_ptr<Window> Initialize(int w, int h, bool fs);
    void clear(void);
    bool swap(void);
private:
    int m_width, m_height;
    GLFWwindow* m_window;
};

#endif /* WINDOW_H */
