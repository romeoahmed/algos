#pragma once

#include <cstddef>
#include <span>
#include <utility>

namespace algos {
// Return the fewest arrows piercing all closed intervals; empty returns zero.
// Requires start <= end for each (start, end) pair; endpoints are included.
[[nodiscard]] auto
burst_balloons(std::span<const std::pair<int, int>> intervals) -> std::size_t;
} // namespace algos
