#include "solution.hpp"

#include <algorithm>
#include <cstddef>
#include <ranges>
#include <vector>

namespace algos {
auto tromino_tiling(std::size_t size, Cell hole) -> std::vector<Tile> {
    auto tiles = std::vector<Tile>{};
    const auto cover = [&tiles](this const auto &self, Cell origin,
                                std::size_t side, Cell missing) -> void {
        if (side == 1) {
            return;
        }
        const auto [top, left] = origin;
        const auto half = side / 2;
        const auto quadrant =
            2 * static_cast<std::size_t>(missing.row >= top + half) +
            static_cast<std::size_t>(missing.column >= left + half);
        const auto quadrants = std::views::iota(std::size_t{0}, std::size_t{4});
        const auto center = [top, left, half](std::size_t q) -> Cell {
            return {.row = top + half - 1 + q / 2,
                    .column = left + half - 1 + q % 2};
        };
        auto tile = Tile{};
        std::ranges::copy(
            quadrants | std::views::filter([quadrant](auto q) -> bool {
                return q != quadrant;
            }) | std::views::transform(center),
            tile.begin());
        tiles.push_back(tile);
        for (const auto q : quadrants) {
            self(Cell{.row = top + q / 2 * half, .column = left + q % 2 * half},
                 half, q == quadrant ? missing : center(q));
        }
    };
    cover(Cell{.row = 0, .column = 0}, size, hole);
    return tiles;
}
} // namespace algos
