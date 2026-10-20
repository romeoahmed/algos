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
// Explore every route forward from each start without sharing results.
auto search_routes(const std::vector<int> &costs) -> std::int64_t {
    const auto leave = [&costs](this const auto &self,
                                std::size_t i) -> std::int64_t {
        if (i >= costs.size()) {
            return 0;
        }
        return costs.at(i) + std::min(self(i + 1), self(i + 2));
    };
    return std::min(leave(0), leave(1));
}
} // namespace

auto main() -> int try {
    using namespace boost::ut;
    constexpr auto high = std::numeric_limits<int>::max();

    "matches route search"_test = [] -> void {
        expect(rc::check([] -> void {
            const auto size = *rc::gen::inRange<std::size_t>(2, 17);
            auto costs = *rc::gen::container<std::vector<int>>(
                size, rc::gen::inRange(0, 1001));
            const auto before = costs;
            RC_ASSERT(algos::min_cost_stairs(costs) == search_routes(costs));
            RC_ASSERT(costs == before);
        }));
    };

    "two steps, avoided peaks, and integer limits"_test =
        [](const auto &example) -> void {
        const auto &[costs, expected] = example;
        expect(that % algos::min_cost_stairs(costs) == expected);
    } | std::vector<std::pair<std::vector<int>, std::int64_t>>{
            {{0, 0}, 0},
            {{5, 3}, 3},
            {{10, 15, 20}, 15},
            {{1, 100, 1, 1, 1, 100, 1, 1, 100, 1}, 6},
            {{high, high, high, high}, 2 * std::int64_t{high}}};
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
