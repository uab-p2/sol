#pragma once

#include <string>
#include <vector>

/// Apply operation A to `message`: the central element stays where
/// it is, and the two blocks of `(N-1)/2` elements around it are
/// swapped, each block keeping its internal order.
/// @param message the message to transform, with `N` elements, `N`
///   odd. It is left untouched.
/// @param left set to the first `(N-1)/2` elements of the result.
/// @param centre set to the central element of the result.
/// @param right set to the last `(N-1)/2` elements of the result.
void operation_A(const std::vector<int>& message,
                 std::vector<int>& left,
                 int& centre,
                 std::vector<int>& right);

/// Undo one application of operation A. Swapping the two blocks
/// around the centre twice restores the message, so the operation is
/// its own inverse.
/// @param left the first `(N-1)/2` elements of the transformed
///   message.
/// @param centre its central element.
/// @param right its last `(N-1)/2` elements.
/// @param message set to the message operation A was applied to.
void inverse_operation_A(const std::vector<int>& left,
                         int centre,
                         const std::vector<int>& right,
                         std::vector<int>& message);

/// Apply operation B to `message`: every element moves one position
/// to the right, and the element that was last becomes the first
/// one. The message keeps its size.
/// @param message the message to transform, with `N` elements, `N`
///   odd. It is left untouched.
/// @param left set to the first `(N-1)/2` elements of the result.
/// @param centre set to the central element of the result.
/// @param right set to the last `(N-1)/2` elements of the result.
void operation_B(const std::vector<int>& message,
                 std::vector<int>& left,
                 int& centre,
                 std::vector<int>& right);

/// Undo one application of operation B: every element moves one
/// position to the left, and the element that was first becomes the
/// last one.
/// @param left the first `(N-1)/2` elements of the transformed
///   message.
/// @param centre its central element.
/// @param right its last `(N-1)/2` elements.
/// @param message set to the message operation B was applied to.
void inverse_operation_B(const std::vector<int>& left,
                         int centre,
                         const std::vector<int>& right,
                         std::vector<int>& message);

/// Apply operation C to `message`: the central element stays where
/// it is, and the left and right blocks of `(N-1)/2` elements are
/// added up one by one (first with first, second with second, and so
/// on). The left block is left untouched; the right block is
/// overwritten with the sums.
/// @param message the message to transform, with `N` elements, `N`
///   odd. It is left untouched.
/// @param left set to the first `(N-1)/2` elements of the result.
/// @param centre set to the central element of the result.
/// @param right set to the last `(N-1)/2` elements of the result.
void operation_C(const std::vector<int>& message,
                 std::vector<int>& left,
                 int& centre,
                 std::vector<int>& right);

/// Undo one application of operation C: the left block is subtracted
/// from the right one, element by element, recovering the right
/// block that the operation added it into.
/// @param left the first `(N-1)/2` elements of the transformed
///   message.
/// @param centre its central element.
/// @param right its last `(N-1)/2` elements.
/// @param message set to the message operation C was applied to.
void inverse_operation_C(const std::vector<int>& left,
                         int centre,
                         const std::vector<int>& right,
                         std::vector<int>& message);

/// Hide `message` by applying to it, one after another, the
/// operations listed in `key`: `operation_A`, `operation_B` and
/// `operation_C`, each repeated as many times as the key says.
/// @param message the message to hide, with `N` elements, `N` odd
///   and `N >= 3`. It is encrypted in place.
/// @param key the operations to apply, each written as a letter
///   (`A`, `B` or `C`) followed by how many times that operation is
///   repeated, a single digit between 1 and 9, e.g. `A1C2A1B3`. The
///   operations are applied from left to right, and each letter may
///   appear any number of times and in any order.
void nanobot_encrypt(std::vector<int>& message, const std::string& key);

/// Recover a message that `nanobot_encrypt` hid using the same
/// `key`, by undoing every operation of the key in reverse order.
/// @param message the encrypted message, with `N` elements, `N` odd
///   and `N >= 3`. It is decrypted in place.
/// @param key the key the message was encrypted with, in the very
///   same format `nanobot_encrypt` expects.
void nanobot_decrypt(std::vector<int>& message, const std::string& key);
