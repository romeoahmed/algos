#include "solution.hpp"

#include <algorithm>
#include <cstddef>
#include <optional>
#include <ranges>
#include <span>
#include <utility>

namespace algos {
auto burst_balloons(std::span<const std::pair<int, int>> intervals)
    -> std::size_t {
    const auto choose = [intervals](this const auto &self,
                                    std::optional<int> boundary,
                                    std::size_t arrows) -> std::size_t {
        auto ends =
            intervals |
            std::views::filter([boundary](const auto &interval) -> bool {
                return !boundary || interval.first > *boundary;
            }) |
            std::views::values;
        const auto earliest = std::ranges::min_element(ends);
        return earliest == ends.end() ? arrows : self(*earliest, arrows + 1);
    };
    return choose(std::nullopt, 0);
}
} // namespace algos
