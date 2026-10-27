#include "solution.hpp"

#include <algorithm>
#include <cstddef>
#include <functional>
#include <ranges>
#include <string_view>
#include <vector>

namespace algos {
auto longest_common(std::string_view first, std::string_view second)
    -> std::size_t {
    // Roll over the shorter string; equal lengths keep the given order.
    const auto [shorter, longer] =
        std::ranges::minmax({first, second}, std::less{}, std::ranges::size);
    const auto advance = [shorter](const std::vector<std::size_t> &above,
                                   char current) -> std::vector<std::size_t> {
        auto row = std::vector<std::size_t>{0};
        row.reserve(above.size());
        // Each cell sees its column character and the (diagonal, up) pair.
        for (const auto &[other, corner] :
             std::views::zip(shorter, above | std::views::pairwise)) {
            const auto [diagonal, up] = corner;
            row.push_back(current == other ? diagonal + 1
                                           : std::max(up, row.back()));
        }
        return row;
    };
    return std::ranges::fold_left(
               longer, std::vector<std::size_t>(shorter.size() + 1, 0), advance)
        .back();
}
} // namespace algos
