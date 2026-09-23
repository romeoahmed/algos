#pragma once

#include <string>
#include <string_view>

namespace algos {
// Returns the exact decimal product without leading zeros, except for "0".
// Requires nonempty ASCII digit strings; leading zeros are allowed.
[[nodiscard]] auto karatsuba(std::string_view x, std::string_view y)
    -> std::string;
} // namespace algos
