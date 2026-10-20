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
// Sum every nonempty contiguous slice.
auto brute_force(const std::vector<int> &values) -> std::int64_t {
    auto best = std::numeric_limits<std::int64_t>::min();
    for (auto start = std::size_t{0}; start < values.size(); ++start) {
        auto total = std::int64_t{0};
        for (auto stop = start; stop < values.size(); ++stop) {
            total += values.at(stop);
            best = std::max(best, total);
        }
    }
    return best;
}
} // namespace

auto main() -> int try {
    using namespace boost::ut;
    constexpr auto low = std::numeric_limits<int>::min();
    constexpr auto high = std::numeric_limits<int>::max();

    "matches brute force"_test = [] -> void {
        expect(rc::check([] -> void {
            const auto size = *rc::gen::inRange<std::size_t>(1, 31);
            auto values = *rc::gen::container<std::vector<int>>(
                size, rc::gen::arbitrary<int>());
            const auto before = values;
            RC_ASSERT(algos::max_subarray(values) == brute_force(values));
            RC_ASSERT(values == before);
        }));
    };

    "negatives, restarts, whole arrays, and integer limits"_test =
        [](const auto &example) -> void {
        const auto &[values, expected] = example;
        expect(that % algos::max_subarray(values) == expected);
    } | std::vector<std::pair<std::vector<int>, std::int64_t>>{
            {{-3}, -3},
            {{-3, -1, -2}, -1},
            {{-2, 1, -3, 4, -1, 2, 1, -5, 4}, 6},
            {{5, 4, -1, 7, 8}, 23},
            {{3, -4, 3}, 3},
            {{low, low}, low},
            {{high, high}, 2 * std::int64_t{high}}};
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
