#pragma once

#include <cstddef>
#include <string_view>

namespace algos {
// Return the longest substring length with every letter occurring at least k
// times. Requires lowercase ASCII letters; empty returns zero. For k <= 1,
// return text.size().
[[nodiscard]] auto longest_substring(std::string_view text, int k)
    -> std::size_t;
} // namespace algos
