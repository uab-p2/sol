#pragma once

#include <vector>

/// Determine, via the Havel-Hakimi algorithm, whether some simple
/// undirected graph has vertices with exactly the degrees in
/// `degrees`. At each step, the vertex with the most pending
/// connections is connected to as many of the other vertices with
/// the most pending connections as it needs.
/// @param degrees a candidate graph degree sequence, sorted in
///   descending order: `degrees[i]` is how many other vertices the
///   i-th vertex should connect to.
/// @return true if `degrees` is graphical, i.e. some simple
///   undirected graph has exactly these vertex degrees; false
///   otherwise, including when `degrees` is empty, not sorted in
///   descending order, or contains a zero or negative element.
bool havel_hakimi(const std::vector<int>& degrees);
