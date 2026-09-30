#include "solution.hpp"

#include <algorithm>
#include <cstddef>
#include <optional>
#include <ranges>
#include <span>
#include <utility>

namespace algos {
auto erase_overlap(std::span<const std::pair<int, int>> intervals)
    -> std::size_t {
    const auto choose = [intervals](this const auto &self,
                                    std::optional<int> boundary,
                                    std::size_t kept) -> std::size_t {
        auto ends =
            intervals |
            std::views::filter([boundary](const auto &interval) -> bool {
                return !boundary || interval.first >= *boundary;
            }) |
            std::views::values;
        const auto earliest = std::ranges::min_element(ends);
        return earliest == ends.end() ? kept : self(*earliest, kept + 1);
    };
    return intervals.size() - choose(std::nullopt, 0);
}
} // namespace algos
