#include "solution.hpp"

#include <cstddef>
#include <ranges>
#include <span>
#include <utility>
#include <vector>

namespace algos {
namespace {
auto partition(std::span<const int> values, int pivot)
    -> std::pair<std::vector<int>, std::vector<int>> {
    return {values | std::views::filter([pivot](int value) -> bool {
                return value > pivot;
            }) | std::ranges::to<std::vector>(),
            values | std::views::filter([pivot](int value) -> bool {
                return value < pivot;
            }) | std::ranges::to<std::vector>()};
}
} // namespace

auto quickselect(std::span<const int> values, std::ptrdiff_t k) -> int {
    const auto pivot = values.subspan(values.size() / 2).front();
    const auto [greater, lower] = partition(values, pivot);
    const auto upper_count = std::ssize(values) - std::ssize(lower);
    if (k <= std::ssize(greater)) {
        return quickselect(greater, k);
    }
    if (k <= upper_count) {
        return pivot;
    }
    return quickselect(lower, k - upper_count);
}
} // namespace algos
