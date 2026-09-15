#pragma once

#include <vector>

/// Sum `values`, then add the repetition index to every element,
/// `repeats` times over, keeping only the last sum. `values` itself
/// is never modified: a local copy accumulates the changes across
/// passes instead.
/// @param values sequence to sum and transform, repeatedly.
/// @param repeats number of times to sum and add the repetition
///   index to every element.
/// @return the last sum computed (0 if `values` is empty or
///   `repeats` is 0).
int sum_race(const std::vector<int>& values, int repeats);

/// Search `values` for `first_search`, then search for the square of
/// the position found plus 1 instead, `repeats` times over.
/// @param values sequence to search, sorted ascending.
/// @param first_search number to search for on the first pass.
/// @param repeats number of searches to chain.
/// @return the last position found (-1 if not found).
int linear_race(const std::vector<int>& values, int first_search, int repeats);

/// Search `values` for `first_search`, then search for the square of
/// the position found plus 1 instead, `repeats` times over.
/// @param values sequence to search, sorted ascending.
/// @param first_search number to search for on the first pass.
/// @param repeats number of searches to chain.
/// @return the last position found (-1 if not found).
int binary_race(const std::vector<int>& values, int first_search, int repeats);
