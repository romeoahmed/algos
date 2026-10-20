#include "solution.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <numeric>
#include <ranges>
#include <vector>

namespace algos {
auto unique_paths(std::size_t rows, std::size_t columns) -> std::uint64_t {
    // Each next row is the prefix sum of the previous one.
    const auto advance = [](std::vector<std::uint64_t> row,
                            std::size_t) -> std::vector<std::uint64_t> {
        std::inclusive_scan(row.begin(), row.end(), row.begin());
        return row;
    };
    const auto shorter = std::min(rows, columns);
    const auto longer = std::max(rows, columns);
    return std::ranges::fold_left(std::views::iota(std::size_t{1}, longer),
                                  std::vector<std::uint64_t>(shorter, 1),
                                  advance)
        .back();
}
} // namespace algos
