#pragma once

#include <cstdint>
#include <span>

namespace algos {
// Return the best profit from buying once and selling on a later day.
// Not trading is allowed, so empty input or falling prices return 0.
[[nodiscard]] auto stock_profit(std::span<const int> prices) noexcept
    -> std::int64_t;
} // namespace algos
