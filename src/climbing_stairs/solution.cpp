#include "solution.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <ranges>
#include <utility>

namespace algos {
auto climbing_stairs(std::size_t n) noexcept -> std::uint64_t {
    // The window holds (dp[i - 1], dp[i]), starting from dp[-1] = 0.
    const auto climb = [](auto window, std::size_t) -> auto {
        const auto [before, current] = window;
        return std::pair{current, before + current};
    };
    return std::ranges::fold_left(std::views::iota(std::size_t{0}, n),
                                  std::pair<std::uint64_t, std::uint64_t>{0, 1},
                                  climb)
        .second;
}
} // namespace algos
