#include "solution.hpp"

#include <algorithm>
#include <boost/ut.hpp>
#include <cstddef>
#include <exception>
#include <functional>
#include <iostream>
#include <limits>
#include <ranges>
#include <rapidcheck.h>
#include <tuple>
#include <vector>

auto main() -> int try {
    using namespace boost::ut;
    constexpr auto low = std::numeric_limits<int>::min();
    constexpr auto high = std::numeric_limits<int>::max();

    "each rank matches descending order"_test = [] -> void {
        expect(rc::check([](std::vector<int> values, int value) -> void {
            values.push_back(value);
            const auto before = values;
            auto expected = values;
            std::ranges::sort(expected, std::greater{});
            for (const auto [rank, result] : std::views::zip(
                     std::views::iota(std::ptrdiff_t{1}), expected)) {
                RC_ASSERT(algos::quickselect(values, rank) == result);
            }
            RC_ASSERT(values == before);
        }));
    };

    "singleton, repeated ranks, and integer limits"_test =
        [](const auto &example) -> void {
        const auto &[values, rank, expected] = example;
        expect(that % algos::quickselect(values, rank) == expected);
    } | std::vector<std::tuple<std::vector<int>, std::ptrdiff_t, int>>{
            {{7}, 1, 7},
            {{7, 7, 7}, 2, 7},
            {{3, 1, 3, 2}, 2, 3},
            {{high, low, high}, 2, high},
            {{high, low, high}, 3, low}};
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
