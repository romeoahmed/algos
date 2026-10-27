#include "solution.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <ranges>
#include <span>
#include <vector>

namespace algos {
auto knapsack(std::span<const Item> items, std::size_t capacity)
    -> std::int64_t {
    // best[j] is the largest value within capacity j using the items so far.
    const auto add = [](std::vector<std::int64_t> best,
                        const Item &item) -> std::vector<std::int64_t> {
        // Pair each cell best[j] with rest = best[j - weight] and update from
        // high j to low, so rest still excludes this item when it is read.
        auto choices =
            std::views::zip(best | std::views::drop(item.weight), best) |
            std::views::reverse;
        for (auto &&[cell, rest] : choices) {
            cell = std::max(cell, rest + item.value);
        }
        return best;
    };
    return std::ranges::fold_left(
               items, std::vector<std::int64_t>(capacity + 1, 0), add)
        .back();
}
} // namespace algos
