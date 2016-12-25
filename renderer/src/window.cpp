#include <window.h>

void Window::Destroy(Window* win)
{
    glfwTerminate();
}

std::unique_ptr<Window> Window::Initialize(int w, int h, bool fs)
{
    std::unique_ptr<Window> win(new Window());
    win->m_width = w;
    win->m_height = h;

    if (!glfwInit()) {
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_DOUBLEBUFFER, 1);
    win->m_window = glfwCreateWindow(win->m_width, win->m_height,
        DEFAULT_WINDOW_TITLE, nullptr, nullptr);
    if (win->m_window == NULL) {
        return nullptr;
    }

    glfwMakeContextCurrent(win->m_window);
    glfwSwapInterval(1);
    gladLoadGL();

    std::stringstream title;
    title << DEFAULT_WINDOW_TITLE << " -- OpenGL " << glGetString(GL_VERSION);
    glfwSetWindowTitle(win->m_window, title.str().c_str());

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    return win;
}

void Window::clear(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Window::swap(void)
{
    glfwSwapBuffers(m_window);
    glfwPollEvents();

    return (!glfwWindowShouldClose(m_window));
}

void Window::setClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}
