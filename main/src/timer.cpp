#include <timer.h>

std::shared_ptr<Timer> Timer::Create(void)
{
    std::shared_ptr<Timer> ret(new Timer());
    if (!ret) {
#ifdef TDR_DEBUG
        std::cerr << "Failed to allocated memory for timer." << std::endl;
#endif
        return nullptr;
    }

#ifdef _WIN32
    QueryPerformanceFrequency(&ret->m_res);
#else
    clock_getres(CLOCK_MONOTONIC, &ret->m_res);
#endif /* _WIN32 */

    ret->m_running = false;
    return ret;
}

double Timer::getResolution(void)
{
    double ret = 0.0;
#ifdef _WIN32
    /* do some math here. */
#else
    ret += static_cast<double>(m_res.tv_sec);
    ret += static_cast<double>(m_res.tv_nsec) /
        static_cast<double>(1000000000.0);
#endif
    return ret;
}

double Timer::getTime(void)
{
    double ret = 0.0;

    if (!m_running) {
#ifdef TDR_DEBUG
        std::cerr << "Timer::getTime(): timer isn't running.  Returning 0.";
        std::cerr << std::endl;
#endif
        return 0.0;
    }

#ifdef _WIN32
    LARGE_INTEGER now;
    double elapsed;

    QueryPerformanceCounter(&now);
    elapsed = now.QuadPart - m_start.QuadPart;
    ret = elapsed / static_cast<double>(m_res.QuadPart);
#else
    struct timespec now;
    time_t sec;
    long nsec;

    clock_gettime(CLOCK_MONOTONIC, &now);
    sec = now.tv_sec - m_start.tv_sec;
    nsec = now.tv_nsec - m_start.tv_nsec;

    ret += static_cast<double>(sec);
    ret += static_cast<double>(nsec) / static_cast<double>(1000000000.0);
#endif

    return ret;
}

void Timer::start(void)
{
#ifdef _WIN32
    QueryPerformanceCounter(&m_start);
#else
    clock_gettime(CLOCK_MONOTONIC, &m_start);
#endif

    m_running = true;
}
