#pragma once

#include <array>
#include <cstddef>
#include <vector>

namespace algos {
struct Cell {
    std::size_t row;
    std::size_t column;

    auto operator==(const Cell &) const -> bool = default;
};
using Tile = std::array<Cell, 3>;

// Returns L-shaped tiles covering the board except for hole, in any order.
// Requires a power-of-two size >= 1 and hole coordinates less than size.
[[nodiscard]] auto tromino_tiling(std::size_t size, Cell hole)
    -> std::vector<Tile>;
} // namespace algos
