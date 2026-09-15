#pragma once

#include <vector>

/// Generates `N` random integers uniformly distributed in [-V, V],
/// with V = 10000.
/// @param N number of values to generate.
/// @param V generation range.
/// @return the generated sequence, in generation order.
std::vector<int> generate_random(int N, int V);

/// Computes the relative frequency of each value in [-V, V] within
/// `values`.
/// @param values sequence to analyze; every value must lie in
///   [-V, V].
/// @param V generation range, matching generate_random's.
/// @return one relative frequency per integer in [-V, V], in
///   ascending order, summing to 1 (all zero if `values` is empty).
std::vector<float> analyze_random(const std::vector<int>& values, int V);
