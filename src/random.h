#pragma once

/// Minimal random number generation utilities.
class Random {
public:
    /// @return a random integer uniformly distributed
    ///   in the range [a, b] (inclusive).
    static int get_int(int a, int b);

    /// @return true with probability `probability`
    ///   (in the range [0, 1]), false otherwise.
    static bool get_bool(double probability = 0.5);
};
