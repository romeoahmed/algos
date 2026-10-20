#include "solution.hpp"

#include <algorithm>
#include <cstdint>
#include <span>
#include <utility>

namespace algos {
auto max_subarray(std::span<const int> values) noexcept -> std::int64_t {
    // The state holds the best sum ending here and the best sum so far.
    const auto extend = [](auto state, int value) -> auto {
        const auto [ending, best] = state;
        const auto next = std::max<std::int64_t>(value, ending + value);
        return std::pair{next, std::max(best, next)};
    };
    const auto first = std::int64_t{values.front()};
    return std::ranges::fold_left(values.subspan(1), std::pair{first, first},
                                  extend)
        .second;
}
} // namespace algos
