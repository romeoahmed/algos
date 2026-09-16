#pragma once

#include <span>

namespace algos {
// Returns the value occurring more than half the time; it must exist.
[[nodiscard]] auto majority(std::span<const int> values) noexcept -> int;
} // namespace algos
