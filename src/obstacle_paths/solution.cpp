#include "solution.hpp"

#include <algorithm>
#include <cstdint>
#include <ranges>
#include <span>
#include <string>
#include <utility>
#include <vector>

namespace algos {
auto obstacle_paths(std::span<const std::string> grid) -> std::uint64_t {
    const auto advance =
        [](std::vector<std::uint64_t> paths,
           const std::string &row) -> std::vector<std::uint64_t> {
        auto left = std::uint64_t{0};
        for (auto &&[cell, count] : std::views::zip(row, paths)) {
            // count still holds the paths from above. An obstacle cuts every
            // path through it; other cells add up and left.
            count = cell == '#' ? 0 : left + count;
            left = count;
        }
        return paths;
    };
    // A virtual row above the grid sends one path into the start cell.
    auto start = std::vector<std::uint64_t>(grid.front().size(), 0);
    start.front() = 1;
    return std::ranges::fold_left(grid, std::move(start), advance).back();
}
} // namespace algos
