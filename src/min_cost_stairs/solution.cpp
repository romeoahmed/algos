#include "solution.hpp"

#include <algorithm>
#include <cstdint>
#include <span>
#include <utility>

namespace algos {
auto min_cost_stairs(std::span<const int> costs) noexcept -> std::int64_t {
    // The window holds (dp[i - 1], dp[i]), the cheapest paid stands.
    const auto climb = [](auto window, int cost) -> auto {
        const auto [before, current] = window;
        return std::pair{current, std::min(before, current) + cost};
    };
    const auto start = std::pair<std::int64_t, std::int64_t>{
        costs.front(), costs.subspan(1).front()};
    const auto [before, current] =
        std::ranges::fold_left(costs.subspan(2), start, climb);
    return std::min(before, current);
}
} // namespace algos
