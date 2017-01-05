#include <window.h>

void Window::Destroy(Window* win)
{
    SDL_GL_DeleteContext(win->m_ctx);
    SDL_DestroyWindow(win->m_window);
    SDL_Quit();
}

std::unique_ptr<Window> Window::Initialize(int w, int h, bool fs)
{
    std::unique_ptr<Window> win(new Window());
    win->m_width = w;
    win->m_height = h;

    if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
#ifdef TDR_DEBUG
        std::cerr << "Failed to initialize SDL2 video subsystem." << std::endl;
#endif
        return nullptr;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    win->m_window = SDL_CreateWindow(DEFAULT_WINDOW_TITLE,
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        win->m_width, win->m_height, SDL_WINDOW_OPENGL);
    if (win->m_window == NULL) {
        return nullptr;
    }

    win->m_ctx = SDL_GL_CreateContext(win->m_window);
    gladLoadGL();

    std::stringstream title;
    title << DEFAULT_WINDOW_TITLE << " -- OpenGL " << glGetString(GL_VERSION);
    SDL_SetWindowTitle(win->m_window, title.str().c_str());

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    return win;
}

void Window::clear(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::swap(void)
{
    SDL_GL_SwapWindow(m_window);
}

void Window::setClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}
