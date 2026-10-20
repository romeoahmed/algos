#pragma once

#include <cstdint>
#include <span>

namespace algos {
// Return the maximum sum of pairwise nonadjacent values.
// Choosing nothing is allowed, so empty input returns 0.
[[nodiscard]] auto house_robber(std::span<const int> values) noexcept
    -> std::int64_t;
} // namespace algos
