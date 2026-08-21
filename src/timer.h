#pragma once

/// A simple mission timer that accumulates elapsed seconds.
class Timer {
public:
    /// Create a new timer with zero elapsed time.
    Timer();

    /// Advance the timer by the given number of seconds.
    void tick(double seconds);

    /// Reset the elapsed time back to zero.
    void reset();

    /// Complete a lap and return its duration. Then reset the timer.
    /// @return the lap duration.
    double lap();

private:
    /// Accumulated seconds since the last reset.
    double m_elapsed;
};
