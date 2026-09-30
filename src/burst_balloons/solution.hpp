#pragma once

#include <cstddef>
#include <span>
#include <utility>

namespace algos {
// Returns the minimum arrow count needed to pierce every closed interval.
// Each pair is (start, end), with start <= end. Both endpoints are included.
// Empty input returns zero. Preserves input.
[[nodiscard]] auto
burst_balloons(std::span<const std::pair<int, int>> intervals) -> std::size_t;
} // namespace algos
