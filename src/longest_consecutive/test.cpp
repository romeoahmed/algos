#include "solution.hpp"

#include <algorithm>
#include <boost/ut.hpp>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <limits>
#include <ranges>
#include <rapidcheck.h>
#include <utility>
#include <vector>

namespace {
// Sort the distinct values and measure the longest run of step 1.
auto scan_sorted(std::vector<int> values) -> std::size_t {
    std::ranges::sort(values);
    const auto [last, end] = std::ranges::unique(values);
    values.erase(last, end);
    auto best = std::size_t{0};
    auto run = std::size_t{0};
    for (auto index = std::size_t{0}; index < values.size(); ++index) {
        const auto continues =
            index > 0 &&
            std::int64_t{values.at(index - 1)} + 1 == values.at(index);
        run = continues ? run + 1 : 1;
        best = std::max(best, run);
    }
    return best;
}
} // namespace

auto main() -> int try {
    using namespace boost::ut;
    constexpr auto low = std::numeric_limits<int>::min();
    constexpr auto high = std::numeric_limits<int>::max();

    "matches the sorted scan"_test = [] -> void {
        expect(rc::check([] -> void {
            auto values = *rc::gen::oneOf(
                rc::gen::container<std::vector<int>>(rc::gen::inRange(-20, 21)),
                rc::gen::arbitrary<std::vector<int>>());
            const auto before = values;
            RC_ASSERT(algos::longest_consecutive(values) ==
                      scan_sorted(values));
            RC_ASSERT(values == before);
        }));
    };

    "examples, duplicates, int limits, and linear time"_test =
        [](const auto &example) -> void {
        const auto &[values, expected] = example;
        expect(that % algos::longest_consecutive(values) == expected);
    } | std::vector<std::pair<std::vector<int>, std::size_t>>{
            {{}, 0},
            {{100, 4, 200, 1, 3, 2}, 4},
            {{0, 3, 7, 2, 5, 8, 4, 6, 0, 1}, 9},
            {{5, 5, 5}, 1},
            {{low, high}, 1},
            {{high - 1, high, low + 1, low}, 2},
            {std::views::iota(0, 100'000) | std::views::reverse |
                 std::ranges::to<std::vector>(),
             100'000}};
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
