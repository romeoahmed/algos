#include "solution.hpp"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <ranges>
#include <span>
#include <unordered_map>
#include <utility>

namespace algos {
auto two_sum(std::span<const int> values, int target)
    -> std::optional<std::pair<std::size_t, std::size_t>> {
    // Value scanned so far -> its latest index. 64-bit keys hold every
    // complement target - value exactly.
    auto seen = std::unordered_map<std::int64_t, std::size_t>{};
    for (const auto [position, value] : std::views::enumerate(values)) {
        const auto j = static_cast<std::size_t>(position);
        const auto need = std::int64_t{target} - value;
        if (const auto match = seen.find(need); match != seen.end()) {
            return std::pair{match->second, j};
        }
        seen.insert_or_assign(value, j);
    }
    return std::nullopt;
}
} // namespace algos
