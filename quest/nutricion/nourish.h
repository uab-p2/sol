#pragma once

#include <vector>

/// Compute the energy nanobots gain from a sequence of feeding
/// tokens. Positive values are upload tokens, negative values are
/// download tokens (their absolute value is the token's size), and
/// zeros are dead time to be ignored.
/// @param tokens the tokens received, in any order.
/// @param upload_tokens set to the size of every upload token found
///   in `tokens`, in the same relative order.
/// @param download_tokens set to the size of every download token
///   found in `tokens`, in the same relative order.
/// @return the total size of every token (upload and download
///   alike, zeros excluded) times the average size of the download
///   tokens (0 if there are none).
float nourish(const std::vector<int>& tokens,
              std::vector<int>& upload_tokens,
              std::vector<int>& download_tokens);
