#pragma once

#include <cstdint>
#include <span>

namespace algos {
// Return the units of water trapped between unit-width bars.
// Requires nonnegative heights; empty input returns 0.
[[nodiscard]] auto rain_water(std::span<const int> heights) -> std::int64_t;
} // namespace algos
