#pragma once

#include <cstdint>
#include <string_view>
#include <vector>

namespace algos {
// Returns all parenthesization results, retaining duplicates in any order.
// Requires 1-20 ASCII characters: operands 0-99 separated by +, -, or *.
[[nodiscard]] auto expressions(std::string_view text)
    -> std::vector<std::int64_t>;
} // namespace algos
