#pragma once

#include <vector>

/// Process a sequence of piece IDs and return a sequence of pieces
/// ready for the next assembly stage.
/// @param ids piece IDs needed for the assembly, possibly out of
///   order and with repeats.
/// @return a new sequence with the same distinct values, sorted in
///   ascending order and with no repeats.
std::vector<int> prepare_assembly(const std::vector<int>& ids);
