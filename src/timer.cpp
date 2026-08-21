#include "timer.h"

Timer::Timer()
    : m_elapsed(0) {
}

void Timer::tick(double seconds) {
    m_elapsed += seconds;
}

void Timer::reset() {
    m_elapsed = 0;
}

double Timer::lap() {
    double elapsed = m_elapsed;
    reset();
    return elapsed;
}
