#pragma once

#include <cstdint>
#include <span>
#include <string>

namespace algos {
// Return the number of right/down paths across a grid avoiding obstacles.
// Rows are equal-length strings in which '#' marks an obstacle.
// Requires at least one row and one column, and that the obstacle-free grid
// of the same size has a path count that fits in uint64_t, such as 34 by 34.
[[nodiscard]] auto obstacle_paths(std::span<const std::string> grid)
    -> std::uint64_t;
} // namespace algos
