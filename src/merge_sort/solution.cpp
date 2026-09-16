#include "solution.hpp"

#include <ranges>
#include <span>
#include <vector>

namespace algos {
namespace {
auto merge(std::span<const int> left, std::span<const int> right)
    -> std::vector<int> {
    auto result = std::vector<int>{};
    while (!left.empty() && !right.empty()) {
        auto &next = left.front() <= right.front() ? left : right;
        result.push_back(next.front());
        next = next.subspan(1);
    }
    result.append_range(left);
    result.append_range(right);
    return result;
}
} // namespace

auto merge_sort(std::span<const int> values) -> std::vector<int> {
    if (values.size() < 2) {
        return values | std::ranges::to<std::vector>();
    }
    const auto middle = values.size() / 2;
    return merge(merge_sort(values.first(middle)),
                 merge_sort(values.subspan(middle)));
}
} // namespace algos
