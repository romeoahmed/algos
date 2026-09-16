#include "solution.hpp"

#include <algorithm>
#include <boost/ut.hpp>
#include <exception>
#include <iostream>
#include <limits>
#include <rapidcheck.h>
#include <utility>
#include <vector>

auto main() -> int try {
    using namespace boost::ut;
    constexpr auto low = std::numeric_limits<int>::min();
    constexpr auto high = std::numeric_limits<int>::max();

    "returns a sorted copy"_test = [] -> void {
        expect(rc::check([](std::vector<int> values) -> void {
            const auto before = values;
            auto expected = values;
            std::ranges::sort(expected);
            auto result = algos::merge_sort(values);
            RC_ASSERT(result == expected);
            result.push_back(0);
            RC_ASSERT(values == before);
        }));
    };

    "empty, singleton, duplicates, and integer limits"_test =
        [](const auto &example) -> void {
        const auto &[values, expected] = example;
        expect(that % algos::merge_sort(values) == expected);
    } | std::vector<std::pair<std::vector<int>, std::vector<int>>>{
            {{}, {}},
            {{7}, {7}},
            {{2, 1}, {1, 2}},
            {{3, 1, 2, 1}, {1, 1, 2, 3}},
            {{high, low, 0, low}, {low, low, 0, high}}};
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
