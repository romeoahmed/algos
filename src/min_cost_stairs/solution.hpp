#pragma once

#include <cstdint>
#include <span>

namespace algos {
// Return the minimum total cost to climb past the last step.
// Start on step 0 or 1; after paying costs[i], move up one or two steps.
// Requires at least two nonnegative costs.
[[nodiscard]] auto min_cost_stairs(std::span<const int> costs) noexcept
    -> std::int64_t;
} // namespace algos
