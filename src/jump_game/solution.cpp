#include "solution.hpp"

#include <algorithm>
#include <cstddef>
#include <ranges>
#include <span>

namespace algos {
auto jump_game(std::span<const std::size_t> values) -> std::size_t {
    const auto reach = [values](std::size_t i) -> std::size_t {
        // Clamp before adding: a step may be size_t's maximum.
        const auto remaining = values.size() - 1 - i;
        return i + std::min(values.subspan(i).front(), remaining) + 1;
    };
    const auto expand = [values, &reach](this const auto &self,
                                         std::size_t first, std::size_t stop,
                                         std::size_t jumps) -> std::size_t {
        if (stop == values.size()) {
            return jumps;
        }
        const auto following = std::ranges::max(std::views::iota(first, stop) |
                                                std::views::transform(reach));
        return self(stop, following, jumps + 1);
    };
    return expand(0, 1, 0);
}
} // namespace algos
