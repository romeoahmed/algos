#pragma once

#include <cstddef>
#include <optional>
#include <span>
#include <utility>

namespace algos {
// Return indices i < j with values[i] + values[j] == target and the least such
// j, or nullopt if no pair exists. The sum is exact; int overflow cannot match.
[[nodiscard]] auto two_sum(std::span<const int> values, int target)
    -> std::optional<std::pair<std::size_t, std::size_t>>;
} // namespace algos
