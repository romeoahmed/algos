#pragma once

#include <string_view>

namespace algos {
// Return whether every bracket in text closes in properly nested order.
// Characters other than the three opening brackets count as closers.
[[nodiscard]] auto valid_parentheses(std::string_view text) -> bool;
} // namespace algos
