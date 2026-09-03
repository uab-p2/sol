#pragma once
#include <vector>

/// Prints simple vertical-bar histograms to the terminal, in a
/// plain-ASCII box (no non-ASCII glyphs, so it works out of the
/// box on any compiler/console, including MSVC on Windows).
class Histogram {
public:
    /// Print `frequencies` as a vertical bar histogram. If the
    /// frequencies don't add up to 1 (within `tolerance`), the
    /// bars are replaced by an axis and an error message.
    /// @param frequencies relative frequencies to plot. Must add up
    ///   to 1, since each value is drawn as a fraction of the whole.
    /// @param width how many characters wide each bar is.
    /// @param height how many character rows tall the tallest bar
    ///   can be.
    /// @param tolerance largest allowed absolute difference between
    ///   the sum of `frequencies` and 1 before it's treated as an
    ///   error.
    static void print(const std::vector<float>& frequencies,
                      unsigned width = 5, unsigned height = 5,
                      float tolerance = 1e-6f);
};
