#include "solution.hpp"

#include <algorithm>
#include <cstddef>
#include <ranges>
#include <span>
#include <tuple>
#include <vector>

namespace algos {
auto longest_increasing(std::span<const int> values) -> std::size_t {
    const auto longest = [](std::size_t left, std::size_t right) -> auto {
        return std::max(left, right);
    };
    auto endings = std::vector<std::size_t>{};
    endings.reserve(values.size());
    for (const auto value : values) {
        // endings is shorter than values, so zip pairs only earlier positions.
        auto predecessors =
            std::views::zip(values, endings) |
            std::views::filter([value](const auto &candidate) -> bool {
                return std::get<0>(candidate) < value;
            }) |
            std::views::elements<1>;
        endings.push_back(
            1 + std::ranges::fold_left(predecessors, std::size_t{0}, longest));
    }
    return std::ranges::fold_left(endings, std::size_t{0}, longest);
}
} // namespace algos
