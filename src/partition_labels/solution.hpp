#pragma once

#include <cstddef>
#include <string_view>
#include <vector>

namespace algos {
// Return partition lengths in text order, maximizing the number of parts.
// Each letter stays in one part. Requires lowercase ASCII letters.
// Empty input returns an empty vector.
[[nodiscard]] auto partition_labels(std::string_view text)
    -> std::vector<std::size_t>;
} // namespace algos
