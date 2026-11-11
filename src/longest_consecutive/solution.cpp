#include "solution.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <ranges>
#include <span>
#include <unordered_set>

namespace algos {
auto longest_consecutive(std::span<const int> values) -> std::size_t {
    // 64-bit keys let value - 1 and value + 1 step past either end of int.
    const auto present =
        values | std::ranges::to<std::unordered_set<std::int64_t>>();
    const auto starts_run = [&](std::int64_t value) -> bool {
        return !present.contains(value - 1);
    };
    const auto run_from = [&](std::int64_t start) -> std::size_t {
        auto end = start + 1;
        while (present.contains(end)) {
            ++end;
        }
        return static_cast<std::size_t>(end - start);
    };
    return std::ranges::fold_left(
        present | std::views::filter(starts_run) |
            std::views::transform(run_from),
        std::size_t{0}, [](std::size_t best, std::size_t run) -> std::size_t {
            return std::max(best, run);
        });
}
} // namespace algos
