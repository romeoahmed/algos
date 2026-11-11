#pragma once

#include <cstddef>
#include <span>

namespace algos {
// Return the length of the longest run of consecutive distinct values; runs
// may touch either end of int.
[[nodiscard]] auto longest_consecutive(std::span<const int> values)
    -> std::size_t;
} // namespace algos
