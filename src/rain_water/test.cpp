#include "solution.hpp"

#include <algorithm>
#include <boost/ut.hpp>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <limits>
#include <rapidcheck.h>
#include <span>
#include <utility>
#include <vector>

namespace {
// Count each unit cell below a level that has a wall on both sides.
auto fill_by_levels(const std::vector<int> &heights) -> std::int64_t {
    const auto bars = std::span{heights};
    const auto top = heights.empty() ? 0 : std::ranges::max(heights);
    auto water = std::int64_t{0};
    for (auto level = 1; level <= top; ++level) {
        const auto wall = [level](int height) -> bool {
            return height >= level;
        };
        for (auto i = std::size_t{0}; i < bars.size(); ++i) {
            if (heights.at(i) < level &&
                std::ranges::any_of(bars.first(i), wall) &&
                std::ranges::any_of(bars.subspan(i + 1), wall)) {
                ++water;
            }
        }
    }
    return water;
}
} // namespace

auto main() -> int try {
    using namespace boost::ut;
    constexpr auto high = std::numeric_limits<int>::max();

    "matches level-by-level filling"_test = [] -> void {
        expect(rc::check([] -> void {
            const auto size = *rc::gen::inRange<std::size_t>(0, 31);
            auto heights = *rc::gen::container<std::vector<int>>(
                size, rc::gen::inRange(0, 9));
            const auto before = heights;
            RC_ASSERT(algos::rain_water(heights) == fill_by_levels(heights));
            RC_ASSERT(heights == before);
        }));
    };

    "empty, monotone, lower walls, and integer limits"_test =
        [](const auto &example) -> void {
        const auto &[heights, expected] = example;
        expect(that % algos::rain_water(heights) == expected);
    } | std::vector<std::pair<std::vector<int>, std::int64_t>>{
            {{}, 0},
            {{3}, 0},
            {{1, 2, 3}, 0},
            {{3, 2, 1}, 0},
            {{3, 0, 3}, 3},
            {{0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1}, 6},
            {{4, 2, 0, 3, 2, 5}, 9},
            {{5, 0, 0, 1}, 2},
            {{high, 0, 0, high}, 2 * std::int64_t{high}}};
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
