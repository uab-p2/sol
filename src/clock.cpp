#include "clock.h"
#include <chrono>
#include <ctime>

Clock::Clock()
    : m_running(false),
      m_measured(0),
      m_start(0) {
}

Clock::~Clock() {
}

void Clock::start() {
    if (!m_running) {
        m_start = read_seconds();
        m_running = true;
    }
}

void Clock::pause() {
    if (m_running) {
        m_measured += read_seconds() - m_start;
        m_running = false;
    }
}

void Clock::reset() {
    m_running = false;
    m_measured = 0;
    m_start = 0;
}

double Clock::get_seconds() const {
    double seconds = m_measured;

    if (m_running) {
        seconds += read_seconds() - m_start;
    }

    return seconds;
}

double WallClock::read_seconds() const {
    const std::chrono::steady_clock::duration since_epoch =
        std::chrono::steady_clock::now().time_since_epoch();

    return std::chrono::duration<double>(since_epoch).count();
}

double CpuWorkClock::read_seconds() const {
    return static_cast<double>(std::clock()) / CLOCKS_PER_SEC;
}
