#include "solution.hpp"

#include <algorithm>
#include <boost/ut.hpp>
#include <cstddef>
#include <exception>
#include <iostream>
#include <limits>
#include <optional>
#include <rapidcheck.h>
#include <utility>
#include <vector>

namespace {
// Check every subsequence, including the empty one.
auto exhaustive(const std::vector<int> &values) -> std::size_t {
    auto best = std::size_t{0};
    for (auto mask = std::size_t{0}; mask < std::size_t{1} << values.size();
         ++mask) {
        auto previous = std::optional<int>{};
        auto size = std::size_t{0};
        auto increasing = true;
        for (auto i = std::size_t{0}; i < values.size() && increasing; ++i) {
            if ((mask >> i & 1U) == 0) {
                continue;
            }
            increasing = !previous || *previous < values.at(i);
            previous = values.at(i);
            ++size;
        }
        if (increasing) {
            best = std::max(best, size);
        }
    }
    return best;
}
} // namespace

auto main() -> int try {
    using namespace boost::ut;
    constexpr auto low = std::numeric_limits<int>::min();
    constexpr auto high = std::numeric_limits<int>::max();

    "matches exhaustive search"_test = [] -> void {
        expect(rc::check([] -> void {
            const auto size = *rc::gen::inRange<std::size_t>(0, 13);
            auto values = *rc::gen::container<std::vector<int>>(
                size, rc::gen::inRange(-6, 7));
            const auto before = values;
            RC_ASSERT(algos::longest_increasing(values) == exhaustive(values));
            RC_ASSERT(values == before);
        }));
    };

    "empty, equal values, skipped neighbors, and integer limits"_test =
        [](const auto &example) -> void {
        const auto &[values, expected] = example;
        expect(that % algos::longest_increasing(values) == expected);
    } | std::vector<std::pair<std::vector<int>, std::size_t>>{
            {{}, 0},
            {{4}, 1},
            {{7, 7, 7}, 1},
            {{10, 9, 2, 5, 3, 7, 101, 18}, 4},
            {{1, 100, 2, 3}, 3},
            {{0, 1, 0, 3, 2, 3}, 4},
            {{high, low, 0, high}, 3}};
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
