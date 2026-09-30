#pragma once

#include <cstddef>
#include <string_view>
#include <vector>

namespace algos {
// Returns lengths, in order, of the maximum number of contiguous parts.
// All occurrences of each letter must belong to one part.
// Requires lowercase ASCII letters. Empty input returns an empty vector.
[[nodiscard]] auto partition_labels(std::string_view text)
    -> std::vector<std::size_t>;
} // namespace algos
