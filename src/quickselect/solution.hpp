#pragma once

#include <cstddef>
#include <span>

namespace algos {
// Returns the k-th largest value, counting duplicates.
// Requires 1 <= k <= values.size().
[[nodiscard]] auto quickselect(std::span<const int> values, std::ptrdiff_t k)
    -> int;
} // namespace algos
