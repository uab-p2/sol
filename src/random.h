#pragma once

/// Minimal random number generation utilities.
class Random {
public:
    /// @return a random integer uniformly distributed
    ///   in the range [a, b] (inclusive).
    ///
    /// Example:
    /// @code
    /// int roll = Random::get_int(1, 6);
    /// @endcode
    static int get_int(int a, int b);

    /// @return true with probability `probability`
    ///   (in the range [0, 1]), false otherwise.
    ///
    /// Example:
    /// @code
    /// if (Random::get_bool(0.1)) {
    ///     std::cout << "rare event" << std::endl;
    /// }
    /// @endcode
    static bool get_bool(double probability = 0.5);
};
