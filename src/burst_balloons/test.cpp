#include "solution.hpp"

#include <algorithm>
#include <bit>
#include <boost/ut.hpp>
#include <cstddef>
#include <exception>
#include <iostream>
#include <limits>
#include <ranges>
#include <rapidcheck.h>
#include <span>
#include <utility>
#include <vector>

namespace {
// Any arrow can move to the smallest right endpoint among the intervals it
// hits.
auto minimum_cover(const std::vector<std::pair<int, int>> &intervals)
    -> std::size_t {
    auto best = intervals.size();
    const auto indices = std::views::iota(std::size_t{0}, intervals.size());
    for (auto mask = 0U; mask < (1U << intervals.size()); ++mask) {
        auto points = indices |
                      std::views::filter([mask](std::size_t i) -> bool {
                          return (mask & (1U << i)) != 0;
                      }) |
                      std::views::transform([&intervals](std::size_t i) -> int {
                          return intervals.at(i).second;
                      });
        const auto covered = std::ranges::all_of(
            intervals, [&points](const auto &interval) -> bool {
                return std::ranges::any_of(points,
                                           [&interval](int point) -> bool {
                                               return interval.first <= point &&
                                                      point <= interval.second;
                                           });
            });
        if (covered) {
            best =
                std::min(best, static_cast<std::size_t>(std::popcount(mask)));
        }
    }
    return best;
}
} // namespace

auto main() -> int try {
    using namespace boost::ut;
    "matches exhaustive subsets and preserves input"_test = [] -> void {
        expect(rc::check([] -> void {
            const auto size = *rc::gen::inRange<std::size_t>(0, 8);
            auto intervals =
                *rc::gen::container<std::vector<std::pair<int, int>>>(
                    size, rc::gen::pair(rc::gen::inRange(-12, 13),
                                        rc::gen::inRange(0, 9)));
            for (auto &[start, end] : intervals) {
                end += start;
            }
            const auto before = intervals;
            const auto expected = minimum_cover(intervals);
            RC_ASSERT(algos::burst_balloons(intervals) == expected);
            RC_ASSERT(intervals == before);
            std::ranges::reverse(intervals);
            const auto reversed = intervals;
            RC_ASSERT(algos::burst_balloons(intervals) == expected);
            RC_ASSERT(intervals == reversed);
        }));
    };
    constexpr auto low = std::numeric_limits<int>::min();
    constexpr auto high = std::numeric_limits<int>::max();
    "boundaries, endpoint equality, and greedy counterexamples"_test =
        [](const auto &example) -> void {
        const auto &[intervals, expected] = example;
        expect(that % algos::burst_balloons(intervals) == expected);
    } | std::vector<std::pair<std::vector<std::pair<int, int>>, std::size_t>>{
            {{}, 0},
            {{{1, 1}}, 1},
            {{{10, 16}, {2, 8}, {1, 6}, {7, 12}}, 2},
            {{{1, 2}, {2, 3}}, 1},
            {{{1, 2}, {2, 3}, {3, 4}}, 2},
            {{{1, 2}, {1, 2}, {1, 2}}, 1},
            {{{0, 10}, {1, 2}, {8, 9}}, 2},
            {{{1, 1}, {2, 2}}, 2},
            {{{low, 0}, {0, high}}, 1}};
    "reads only the supplied subspan"_test = [] -> void {
        const auto intervals =
            std::vector<std::pair<int, int>>{{-5, -4}, {1, 2}, {2, 3}, {4, 4}};
        expect(that %
                   algos::burst_balloons(std::span{intervals}.subspan(1, 2)) ==
               1U);
        expect(that %
                   algos::burst_balloons(std::span{intervals}.subspan(1, 0)) ==
               0U);
    };
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
