#pragma once

#include <cstddef>
#include <span>
#include <utility>

namespace algos {
// Returns the minimum removals needed to leave nonoverlapping intervals.
// Each pair is (start, end), with start < end; touching endpoints are allowed.
// Empty input returns zero. Preserves input and counts duplicates separately.
[[nodiscard]] auto erase_overlap(std::span<const std::pair<int, int>> intervals)
    -> std::size_t;
} // namespace algos
