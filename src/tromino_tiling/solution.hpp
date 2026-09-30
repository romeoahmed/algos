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

// Return L-shaped tiles covering every cell except hole exactly once.
// Tile and cell order are unspecified; a size-one board returns no tiles.
// Requires a power-of-two size >= 1 and zero-based hole coordinates below size.
[[nodiscard]] auto tromino_tiling(std::size_t size, Cell hole)
    -> std::vector<Tile>;
} // namespace algos
