#pragma once

#include <cstddef>
#include <span>

namespace algos {
// Return the length of a longest strictly increasing subsequence.
// Empty input returns 0.
[[nodiscard]] auto longest_increasing(std::span<const int> values)
    -> std::size_t;
} // namespace algos
