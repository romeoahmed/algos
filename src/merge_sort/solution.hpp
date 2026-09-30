#pragma once

#include <span>
#include <vector>

namespace algos {
// Return a stably sorted copy in nondecreasing order; empty is allowed.
[[nodiscard]] auto merge_sort(std::span<const int> values) -> std::vector<int>;
} // namespace algos
