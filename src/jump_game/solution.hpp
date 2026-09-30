#pragma once

#include <cstddef>
#include <span>

namespace algos {
// Returns the minimum jump count; a singleton needs zero jumps.
// Each value is the maximum forward step from that index.
// Requires nonempty input with a reachable last index. Preserves input.
[[nodiscard]] auto jump_game(std::span<const std::size_t> values)
    -> std::size_t;
} // namespace algos
