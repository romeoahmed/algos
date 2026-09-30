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
auto minimum_removals(const std::vector<std::pair<int, int>> &intervals)
    -> std::size_t {
    auto best = std::size_t{0};
    const auto indices = std::views::iota(std::size_t{0}, intervals.size());
    for (auto mask = 0U; mask < (1U << intervals.size()); ++mask) {
        auto selected =
            indices | std::views::filter([mask](std::size_t i) -> bool {
                return (mask & (1U << i)) != 0;
            });
        const auto valid =
            std::ranges::all_of(selected, [&](std::size_t i) -> bool {
                const auto [start, end] = intervals.at(i);
                return std::ranges::all_of(
                    selected, [&](std::size_t j) -> bool {
                        return i == j || end <= intervals.at(j).first ||
                               intervals.at(j).second <= start;
                    });
            });
        if (valid) {
            best =
                std::max(best, static_cast<std::size_t>(std::popcount(mask)));
        }
    }
    return intervals.size() - best;
}
} // namespace

auto main() -> int try {
    using namespace boost::ut;
    "matches exhaustive subsets and preserves input"_test = [] -> void {
        expect(rc::check([] -> void {
            const auto size = *rc::gen::inRange<std::size_t>(0, 9);
            auto intervals =
                *rc::gen::container<std::vector<std::pair<int, int>>>(
                    size, rc::gen::pair(rc::gen::inRange(-12, 13),
                                        rc::gen::inRange(1, 9)));
            for (auto &[start, end] : intervals) {
                end += start;
            }
            const auto before = intervals;
            const auto expected = minimum_removals(intervals);
            RC_ASSERT(algos::erase_overlap(intervals) == expected);
            RC_ASSERT(intervals == before);
            std::ranges::reverse(intervals);
            const auto reversed = intervals;
            RC_ASSERT(algos::erase_overlap(intervals) == expected);
            RC_ASSERT(intervals == reversed);
        }));
    };
    constexpr auto low = std::numeric_limits<int>::min();
    constexpr auto high = std::numeric_limits<int>::max();
    "boundaries, endpoint equality, and greedy counterexamples"_test =
        [](const auto &example) -> void {
        const auto &[intervals, expected] = example;
        expect(that % algos::erase_overlap(intervals) == expected);
    } | std::vector<std::pair<std::vector<std::pair<int, int>>, std::size_t>>{
            {{}, 0},
            {{{1, 2}}, 0},
            {{{1, 2}, {2, 3}, {3, 4}, {1, 3}}, 1},
            {{{1, 2}, {1, 2}, {1, 2}}, 2},
            {{{0, 10}, {1, 2}, {2, 3}}, 1},
            {{{0, 3}, {2, 4}, {3, 6}}, 1},
            {{{-3, -2}, {-2, -1}}, 0},
            {{{low, 0}, {0, high}}, 0}};
    "reads only the supplied subspan"_test = [] -> void {
        const auto intervals = std::vector<std::pair<int, int>>{
            {-10, 10}, {1, 2}, {2, 3}, {1, 3}, {3, 4}};
        expect(that %
                   algos::erase_overlap(std::span{intervals}.subspan(1, 3)) ==
               1U);
        expect(that %
                   algos::erase_overlap(std::span{intervals}.subspan(1, 0)) ==
               0U);
    };
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
