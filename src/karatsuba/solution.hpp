#pragma once

#include <string>
#include <string_view>

namespace algos {
// Return the exact decimal product, with zero represented as "0".
// Nonzero results have no leading zeros.
// Requires nonempty ASCII digit strings; leading zeros are allowed.
[[nodiscard]] auto karatsuba(std::string_view x, std::string_view y)
    -> std::string;
} // namespace algos
