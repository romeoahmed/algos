#pragma once

#include <span>

namespace algos {
// Return the value occurring strictly more than values.size() / 2 times.
// Requires a nonempty input with such a majority.
[[nodiscard]] auto majority(std::span<const int> values) noexcept -> int;
} // namespace algos
