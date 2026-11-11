#include "solution.hpp"

#include <algorithm>
#include <boost/ut.hpp>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <limits>
#include <optional>
#include <rapidcheck.h>
#include <tuple>
#include <utility>
#include <vector>

namespace {
using Pair = std::pair<std::size_t, std::size_t>;

// Try every pair of indices and keep those summing exactly to target.
auto all_pairs(const std::vector<int> &values, int target)
    -> std::vector<Pair> {
    auto pairs = std::vector<Pair>{};
    for (auto j = std::size_t{0}; j < values.size(); ++j) {
        for (auto i = std::size_t{0}; i < j; ++i) {
            if (std::int64_t{values.at(i)} + values.at(j) == target) {
                pairs.emplace_back(i, j);
            }
        }
    }
    return pairs;
}
} // namespace

auto main() -> int try {
    using namespace boost::ut;
    constexpr auto low = std::numeric_limits<int>::min();
    constexpr auto high = std::numeric_limits<int>::max();

    "finds the earliest completed pair"_test = [] -> void {
        expect(rc::check([] -> void {
            const auto small = rc::gen::inRange(-8, 9);
            auto values =
                *rc::gen::oneOf(rc::gen::container<std::vector<int>>(small),
                                rc::gen::arbitrary<std::vector<int>>());
            const auto target = *rc::gen::oneOf(rc::gen::inRange(-16, 17),
                                                rc::gen::arbitrary<int>());
            const auto before = values;
            const auto pairs = all_pairs(values, target);
            const auto result = algos::two_sum(values, target);
            RC_ASSERT(values == before);
            if (pairs.empty()) {
                RC_ASSERT(!result.has_value());
                return;
            }
            RC_ASSERT(result.has_value());
            const auto [i, j] = *result;
            RC_ASSERT(i < j);
            RC_ASSERT(std::int64_t{values.at(i)} + values.at(j) == target);
            RC_ASSERT(j == std::ranges::min(pairs, {}, &Pair::second).second);
        }));
    };

    "examples, duplicates, and int limits"_test =
        [](const auto &example) -> void {
        const auto &[values, target, expected] = example;
        expect(algos::two_sum(values, target) == expected);
    } | std::vector<std::tuple<std::vector<int>, int, std::optional<Pair>>>{
            {{2, 7, 11, 15}, 9, Pair{0, 1}}, {{3, 2, 4}, 6, Pair{1, 2}},
            {{3, 3}, 6, Pair{0, 1}},         {{3}, 6, std::nullopt},
            {{}, 0, std::nullopt},           {{1, 5, 1, 5}, 6, Pair{0, 1}},
            {{low, high}, -1, Pair{0, 1}},   {{high, 1}, low, std::nullopt},
            {{low, low}, 0, std::nullopt}};
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
