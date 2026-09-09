#pragma once
#include <vector>

/// Hunt `target_id` down in a row of nanobot IDs that may come in
/// any order, taking a single nanobot out of the row.
/// @param ids the IDs of the row, in any order. The same ID may
///   appear several times. If `target_id` is found, only its first
///   occurrence is removed from `ids`, and the remaining IDs keep
///   their relative order. Otherwise `ids` is left untouched.
/// @param target_id the ID to look for. It may or may not appear in
///   `ids`.
/// @return true if `target_id` was found and one nanobot removed.
bool hunt_unordered(std::vector<int>& ids, int target_id);

/// Hunt `target_id` down in a row of nanobot IDs sorted in ascending
/// order, taking every nanobot with that ID out of the row.
/// @param ids the IDs of the row, sorted in ascending order. The
///   same ID may appear several times, and repeated IDs sit next to
///   each other. If `target_id` is found, every one of its
///   occurrences is removed from `ids`, which stays sorted.
///   Otherwise `ids` is left untouched.
/// @param target_id the ID to look for. It may or may not appear in
///   `ids`.
/// @return true if `target_id` was found and removed.
bool hunt_ascending(std::vector<int>& ids, int target_id);
