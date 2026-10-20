#include "solution.hpp"

#include <algorithm>
#include <cstdint>
#include <span>
#include <utility>

namespace algos {
auto house_robber(std::span<const int> values) noexcept -> std::int64_t {
    // The window holds (dp[i - 1], dp[i]) for the prefix values[0:i].
    const auto visit = [](auto window, int value) -> auto {
        const auto [before, best] = window;
        return std::pair{best, std::max(best, before + value)};
    };
    return std::ranges::fold_left(
               values, std::pair<std::int64_t, std::int64_t>{0, 0}, visit)
        .second;
}
} // namespace algos
