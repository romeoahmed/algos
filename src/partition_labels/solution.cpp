#include "solution.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <ranges>
#include <string_view>
#include <vector>

namespace algos {
namespace {
struct Partition {
    std::size_t start = 0;
    std::size_t stop = 0;
    std::vector<std::size_t> lengths;
};
} // namespace

auto partition_labels(std::string_view text) -> std::vector<std::size_t> {
    const auto indices = std::views::iota(std::size_t{0}, text.size());
    auto last = std::array<std::size_t, 26>{};
    for (const auto i : indices) {
        last.at(static_cast<std::size_t>(text.at(i) - 'a')) = i;
    }
    const auto cut = [text, &last](Partition state,
                                   std::size_t i) -> Partition {
        state.stop =
            std::max(state.stop,
                     last.at(static_cast<std::size_t>(text.at(i) - 'a')) + 1);
        if (i + 1 == state.stop) {
            state.lengths.push_back(state.stop - state.start);
            state.start = state.stop;
        }
        return state;
    };
    return std::ranges::fold_left(indices, Partition{}, cut).lengths;
}
} // namespace algos
