#include "solution.hpp"

#include <algorithm>
#include <boost/ut.hpp>
#include <cstddef>
#include <exception>
#include <iostream>
#include <iterator>
#include <limits>
#include <ranges>
#include <rapidcheck.h>
#include <set>
#include <tuple>
#include <vector>

auto main() -> int try {
    using namespace boost::ut;
    constexpr auto low = std::numeric_limits<int>::min();
    constexpr auto high = std::numeric_limits<int>::max();

    "matches linear search after any rotation"_test = [] -> void {
        expect(rc::check([](const std::set<int> &unique, int target) -> void {
            auto values = unique | std::ranges::to<std::vector>();
            if (!values.empty()) {
                const auto pivot =
                    *rc::gen::inRange(std::ptrdiff_t{0}, std::ssize(values));
                std::ranges::rotate(values, std::next(values.begin(), pivot));
            }
            const auto before = values;
            for (const auto [index, value] :
                 std::views::zip(std::views::iota(std::ptrdiff_t{0}), values)) {
                RC_ASSERT(algos::rotated_search(values, value) == index);
            }
            const auto found = std::ranges::find(values, target);
            const auto expected = found == values.end()
                                      ? -1
                                      : std::distance(values.begin(), found);
            RC_ASSERT(algos::rotated_search(values, target) == expected);
            RC_ASSERT(values == before);
        }));
    };

    "empty, singleton, missing target, and integer limits"_test =
        [](const auto &example) -> void {
        const auto &[values, target, expected] = example;
        expect(that % algos::rotated_search(values, target) == expected);
    } | std::vector<std::tuple<std::vector<int>, int, std::ptrdiff_t>>{
            {{}, 0, -1},
            {{0}, 0, 0},
            {{0}, 1, -1},
            {{4, 5, 6, 7, 0, 1, 2}, 3, -1},
            {{high, low}, low, 1}};
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
