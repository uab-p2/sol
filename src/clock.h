/// Stopwatch that measures how long a piece of code takes, in
/// seconds. A clock is created stopped and empty: `start` makes it
/// run, `pause` stops it keeping what it has measured so far, and
/// `reset` empties it back to zero. Subclasses choose which kind
/// of time is measured.
///
/// Example:
/// @code
/// WallClock clock;
/// clock.start();
/// slow_function();
/// clock.pause();
/// std::cout << clock.get_seconds() << " s" << std::endl;
/// @endcode
#pragma once

/// Base class for clocks. Use WallClock or CpuWorkClock instead.
class Clock {
public:
    /// Create a clock that is stopped and has measured nothing.
    Clock();

    /// Needed because virtual methods are defined.
    virtual ~Clock();

    /// Start measuring time. Does nothing if the clock is already
    /// running.
    void start();

    /// Stop measuring time, keeping what has been measured so far.
    /// Does nothing if the clock is not running. A later `start`
    /// resumes the measurement instead of starting a new one.
    void pause();

    /// Stop the clock and throw away the time it had measured.
    void reset();

    /// @return the measured time, in seconds. If the clock is
    ///   running, the time since the last `start` is included.
    double get_seconds() const;

protected:
    /// @return the current reading of the time source being
    ///   measured, in seconds. Only the difference between two
    ///   readings is meaningful.
    virtual double
    read_seconds() const = 0;

    /// True while the clock is measuring.
    bool m_running;
    /// Seconds measured before the last `start`.
    double m_measured;
    /// Reading of `read_seconds` taken at the last `start`.
    double m_start;
};

/// Clock that measures wall time: the real time that goes by,
/// just like a clock hanging on the wall, whatever the program is
/// doing meanwhile.
class WallClock : public Clock {
protected:
    /// @return the current wall time, in seconds.
    double read_seconds() const override;
};

/// Clock that measures processor work: only the time the CPU
/// spends running this program. Time spent waiting (for the user,
/// for a file, for another program) does not count.
class CpuWorkClock : public Clock {
protected:
    /// @return the processor time used so far, in seconds.
    double read_seconds() const override;
};
