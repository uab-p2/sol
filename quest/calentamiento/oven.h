#pragma once

#include <vector>

/// @param temperatures oven temperature readings, in chronological
///   order.
/// @return true if `temperatures` is cooling down, i.e. it never
///   regains temperature at any point in the series (an empty or
///   single-reading sequence counts as cooling).
bool is_oven_cooling(const std::vector<int>& temperatures);
