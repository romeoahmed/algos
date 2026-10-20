#include "solution.hpp"

#include <algorithm>
#include <boost/ut.hpp>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <limits>
#include <rapidcheck.h>
#include <utility>
#include <vector>

namespace {
// Try every subset whose bit mask has no two adjacent bits.
auto exhaustive(const std::vector<int> &values) -> std::int64_t {
    auto best = std::int64_t{0};
    for (auto mask = std::size_t{0}; mask < std::size_t{1} << values.size();
         ++mask) {
        if ((mask & mask >> 1U) != 0) {
            continue;
        }
        auto total = std::int64_t{0};
        for (auto i = std::size_t{0}; i < values.size(); ++i) {
            if ((mask >> i & 1U) != 0) {
                total += values.at(i);
            }
        }
        best = std::max(best, total);
    }
    return best;
}
} // namespace

auto main() -> int try {
    using namespace boost::ut;
    constexpr auto low = std::numeric_limits<int>::min();
    constexpr auto high = std::int64_t{std::numeric_limits<int>::max()};

    "matches exhaustive search"_test = [] -> void {
        expect(rc::check([] -> void {
            const auto size = *rc::gen::inRange<std::size_t>(0, 13);
            auto values = *rc::gen::container<std::vector<int>>(
                size, rc::gen::arbitrary<int>());
            const auto before = values;
            RC_ASSERT(algos::house_robber(values) == exhaustive(values));
            RC_ASSERT(values == before);
        }));
    };

    "empty, negatives, skipped neighbors, and integer limits"_test =
        [](const auto &example) -> void {
        const auto &[values, expected] = example;
        expect(that % algos::house_robber(values) == expected);
    } | std::vector<std::pair<std::vector<int>, std::int64_t>>{
            {{}, 0},
            {{5}, 5},
            {{low}, 0},
            {{1, 2, 3, 1}, 4},
            {{2, 7, 9, 3, 1}, 12},
            {{3, 4, 3}, 6},
            {{2, 1, 1, 2}, 4},
            {{static_cast<int>(high), low, static_cast<int>(high)}, 2 * high}};
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
