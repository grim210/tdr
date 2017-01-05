#ifndef WINDOW_H
#define WINDOW_H

#ifdef TDR_DEBUG
#include <iostream>
#endif

#include <memory>
#include <sstream>

#include <glad/glad.h>
#include <SDL2/SDL.h>

#define DEFAULT_WINDOW_TITLE    ("Renderer")

class Window {
public:
    Window() { };
    virtual ~Window() { };
    static void Destroy(Window* win);
    static std::unique_ptr<Window> Initialize(int w, int h, bool fs);
    void clear(void);
    void swap(void);

    void setClearColor(float r, float g, float b, float a);
private:
    int m_width, m_height;
    SDL_Window* m_window;
    SDL_GLContext m_ctx;
};

#endif /* WINDOW_H */
