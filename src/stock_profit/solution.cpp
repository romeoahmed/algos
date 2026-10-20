#include "solution.hpp"

#include <algorithm>
#include <cstdint>
#include <limits>
#include <span>
#include <utility>

namespace algos {
auto stock_profit(std::span<const int> prices) noexcept -> std::int64_t {
    // The state holds the lowest price so far and the best profit so far.
    const auto trade = [](auto state, int price) -> auto {
        const auto [lowest, best] = state;
        const auto low = std::min(lowest, price);
        return std::pair{low, std::max(best, std::int64_t{price} - low)};
    };
    // The int maximum stands for infinity until the first price replaces it.
    return std::ranges::fold_left(
               prices,
               std::pair{std::numeric_limits<int>::max(), std::int64_t{0}},
               trade)
        .second;
}
} // namespace algos
