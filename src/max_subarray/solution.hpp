#pragma once

#include <cstdint>
#include <span>

namespace algos {
// Return the maximum sum of a nonempty contiguous subarray.
// Requires nonempty input.
[[nodiscard]] auto max_subarray(std::span<const int> values) noexcept
    -> std::int64_t;
} // namespace algos
