#pragma once

#include <cstddef>
#include <cstdint>

namespace algos {
// Return the number of right/down paths across a rows-by-columns grid.
// The dimensions are interchangeable.
// Requires positive dimensions and a result that fits in uint64_t.
[[nodiscard]] auto unique_paths(std::size_t rows, std::size_t columns)
    -> std::uint64_t;
} // namespace algos
