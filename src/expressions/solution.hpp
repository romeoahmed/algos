#pragma once

#include <cstdint>
#include <string_view>
#include <vector>

namespace algos {
// Return all parenthesization results, retaining duplicates in any order.
// Requires a valid 1-20 character ASCII expression: operands 0-99 joined by
// +, -, or *, without whitespace, parentheses, or unary operators.
[[nodiscard]] auto expressions(std::string_view text)
    -> std::vector<std::int64_t>;
} // namespace algos
