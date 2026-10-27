#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

namespace algos {
struct Item {
    std::size_t weight;
    int value;

    auto operator==(const Item &) const -> bool = default;
};

// Return the largest total value of items whose total weight fits.
// Each item is used at most once. Requires nonnegative values;
// choosing nothing is allowed.
[[nodiscard]] auto knapsack(std::span<const Item> items, std::size_t capacity)
    -> std::int64_t;
} // namespace algos
