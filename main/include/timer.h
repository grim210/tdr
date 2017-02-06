#ifndef TDR_TIMER_H
#define TDR_TIMER_H

#ifdef TDR_DEBUG
  #include <iostream>
#endif

#ifdef _WIN32
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
#else
  #include <time.h>
#endif

#include <memory>

class Timer {
public:
    static std::shared_ptr<Timer> Create(void);
    double getResolution(void);
    double getTime(void);
    void start(void);
private:
    bool m_running;
#ifdef _WIN32
    LARGE_INTEGER m_start, m_res;
#else
    struct timespec m_start, m_res;
#endif
};

#endif /* TDR_TIMER_H */
