#pragma once

#include <cstddef>
#include <span>

namespace algos {
// Returns the target's index, or -1 if absent.
// Requires a rotated strictly increasing range; empty is allowed.
[[nodiscard]] auto rotated_search(std::span<const int> values,
                                  int target) noexcept -> std::ptrdiff_t;
} // namespace algos
