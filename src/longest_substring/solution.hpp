#pragma once

#include <cstddef>
#include <string_view>

namespace algos {
// Returns the longest length with each letter occurring at least k times.
// Requires lowercase ASCII letters; empty is allowed.
// For k <= 1, returns text.size().
[[nodiscard]] auto longest_substring(std::string_view text, int k)
    -> std::size_t;
} // namespace algos
