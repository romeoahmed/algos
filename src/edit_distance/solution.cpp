#include "solution.hpp"

#include <algorithm>
#include <cstddef>
#include <functional>
#include <ranges>
#include <string_view>
#include <vector>

namespace algos {
auto edit_distance(std::string_view source, std::string_view target)
    -> std::size_t {
    // Roll over the shorter string; equal lengths keep the given order.
    const auto [shorter, longer] =
        std::ranges::minmax({source, target}, std::less{}, std::ranges::size);
    const auto advance = [shorter](const std::vector<std::size_t> &above,
                                   char current) -> std::vector<std::size_t> {
        // Against the empty prefix, each row deletes one more character.
        auto row = std::vector<std::size_t>{above.front() + 1};
        row.reserve(above.size());
        // Each cell sees its column character and the (diagonal, up) pair.
        for (const auto &[other, corner] :
             std::views::zip(shorter, above | std::views::pairwise)) {
            const auto [diagonal, up] = corner;
            row.push_back(current == other
                              ? diagonal
                              : 1 + std::min({diagonal, up, row.back()}));
        }
        return row;
    };
    // The empty prefix of longer reaches each prefix of shorter by insertions.
    return std::ranges::fold_left(
               longer,
               std::views::iota(std::size_t{0}, shorter.size() + 1) |
                   std::ranges::to<std::vector>(),
               advance)
        .back();
}
} // namespace algos
