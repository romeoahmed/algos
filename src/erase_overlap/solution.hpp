#pragma once

#include <cstddef>
#include <span>
#include <utility>

namespace algos {
// Return the fewest removals leaving nonoverlapping intervals; empty returns
// zero. Requires start < end for each (start, end) pair; touching endpoints are
// allowed. Duplicate intervals count separately.
[[nodiscard]] auto erase_overlap(std::span<const std::pair<int, int>> intervals)
    -> std::size_t;
} // namespace algos
