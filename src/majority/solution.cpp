#include "solution.hpp"

#include <algorithm>
#include <cstddef>
#include <span>
#include <utility>

namespace algos {
auto majority(std::span<const int> values) noexcept -> int {
    const auto vote = [](auto state, int value) -> auto {
        const auto [candidate, balance] = state;
        const auto next = balance == 0 ? value : candidate;
        return std::pair{next, balance + (value == next ? 1 : -1)};
    };
    return std::ranges::fold_left(values, std::pair{0, std::ptrdiff_t{0}}, vote)
        .first;
}
} // namespace algos
