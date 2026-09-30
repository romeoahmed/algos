#include "solution.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <ranges>
#include <string_view>
#include <utility>

namespace algos {
auto longest_substring(std::string_view text, int k) -> std::size_t {
    if (k <= 1) {
        return text.size();
    }
    if (std::cmp_less(text.size(), k)) {
        return 0;
    }
    auto counts = std::array<std::size_t, 26>{};
    for (const auto letter : text) {
        ++counts.at(static_cast<std::size_t>(letter - 'a'));
    }
    const auto separator =
        std::ranges::find_if(text, [&counts, k](char letter) -> bool {
            return std::cmp_less(
                counts.at(static_cast<std::size_t>(letter - 'a')), k);
        });
    if (separator == text.end()) {
        return text.size();
    }
    return std::ranges::max(
        text | std::views::split(*separator) |
        std::views::transform([k](auto part) -> std::size_t {
            return longest_substring(std::string_view{part}, k);
        }));
}
} // namespace algos
