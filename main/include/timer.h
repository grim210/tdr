#ifndef TDR_TIMER_H
#define TDR_TIMER_H

#ifdef TDR_DEBUG
#include <iostream>
#endif

#include <memory>

#include <SDL2/SDL.h>

class Timer {
public:
    static std::shared_ptr<Timer> Create(void);
    double getTime(void);
    void start(void);
private:
    bool m_running;
    uint64_t m_start, m_res;
};

#endif /* TDR_TIMER_H */
