#pragma once

#include <cstddef>
#include <string_view>

namespace algos {
// Return the fewest single-byte edits turning source into target.
// Insertions, deletions, and substitutions each cost 1, so the arguments
// are interchangeable.
[[nodiscard]] auto edit_distance(std::string_view source,
                                 std::string_view target) -> std::size_t;
} // namespace algos
