#pragma once

#include <cstddef>
#include <cstdint>

namespace algos {
// Return the number of 1- and 2-step sequences that climb exactly n stairs.
// Zero stairs have one empty sequence.
// Requires the result to fit in uint64_t, that is, n <= 92.
[[nodiscard]] auto climbing_stairs(std::size_t n) noexcept -> std::uint64_t;
} // namespace algos
