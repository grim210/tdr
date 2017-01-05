#include <timer.h>

std::shared_ptr<Timer> Timer::Create(void)
{
    if (SDL_InitSubSystem(SDL_INIT_TIMER)) {
#ifdef TDR_DEBUG
        std::cerr << "Failed to initialize timer subsystem." << std::endl;
#endif
        return nullptr;
    }

    std::shared_ptr<Timer> ret(new Timer());
    if (!ret) {
#ifdef TDR_DEBUG
        std::cerr << "Failed to allocated memory for timer." << std::endl;
#endif
        return nullptr;
    }

    ret->m_running = false;
    ret->m_res = SDL_GetPerformanceFrequency();

    return ret;
}

double Timer::getTime(void)
{
    if (!m_running) {
        return 0.0;
    }

    uint64_t diff = SDL_GetPerformanceCounter() - m_start;
    return static_cast<double>(diff) / static_cast<double>(m_res);
}

void Timer::start(void)
{
    m_start = SDL_GetPerformanceCounter();
    m_running = true;
}
