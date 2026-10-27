#pragma once

#include <cstddef>
#include <string_view>

namespace algos {
// Return the length of a longest common subsequence of two byte strings.
// The arguments are interchangeable; an empty string gives 0.
[[nodiscard]] auto longest_common(std::string_view first,
                                  std::string_view second) -> std::size_t;
} // namespace algos
