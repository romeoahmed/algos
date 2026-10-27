#include "solution.hpp"

#include <algorithm>
#include <boost/ut.hpp>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <limits>
#include <rapidcheck.h>
#include <tuple>
#include <vector>

namespace {
// Try every subset of items, including the empty one, and keep the best fit.
auto exhaustive(const std::vector<algos::Item> &items, std::size_t capacity)
    -> std::int64_t {
    auto best = std::int64_t{0};
    for (auto mask = std::size_t{0}; mask < std::size_t{1} << items.size();
         ++mask) {
        auto weight = std::size_t{0};
        auto value = std::int64_t{0};
        for (auto i = std::size_t{0}; i < items.size(); ++i) {
            if ((mask >> i & 1U) != 0) {
                weight += items.at(i).weight;
                value += items.at(i).value;
            }
        }
        if (weight <= capacity) {
            best = std::max(best, value);
        }
    }
    return best;
}
} // namespace

auto main() -> int try {
    using namespace boost::ut;
    using algos::Item;
    constexpr auto high = std::numeric_limits<int>::max();

    "matches exhaustive search"_test = [] -> void {
        expect(rc::check([] -> void {
            const auto size = *rc::gen::inRange<std::size_t>(0, 11);
            auto items = *rc::gen::container<std::vector<Item>>(
                size,
                rc::gen::construct<Item>(rc::gen::inRange<std::size_t>(0, 13),
                                         rc::gen::inRange(0, 51)));
            const auto capacity = *rc::gen::inRange<std::size_t>(0, 31);
            const auto before = items;
            RC_ASSERT(algos::knapsack(items, capacity) ==
                      exhaustive(items, capacity));
            RC_ASSERT(items == before);
        }));
    };

    "zero capacity, heavy items, single use, and integer limits"_test =
        [](const auto &example) -> void {
        const auto &[items, capacity, expected] = example;
        expect(that % algos::knapsack(items, capacity) == expected);
    } | std::vector<std::tuple<std::vector<Item>, std::size_t, std::int64_t>>{
            {{}, 5, 0},
            {{{.weight = 0, .value = 4}, {.weight = 1, .value = 9}}, 0, 4},
            {{{.weight = 6, .value = 10}}, 5, 0},
            {{{.weight = 1, .value = 1}}, 4, 1},
            {{{.weight = 2, .value = 3},
              {.weight = 3, .value = 4},
              {.weight = 4, .value = 5}},
             5,
             7},
            {{{.weight = 10, .value = 60},
              {.weight = 20, .value = 100},
              {.weight = 30, .value = 120}},
             50,
             220},
            {{{.weight = 5, .value = 10},
              {.weight = 4, .value = 40},
              {.weight = 6, .value = 30},
              {.weight = 3, .value = 50}},
             10,
             90},
            {std::vector<Item>(200, {.weight = 1, .value = 1}), 100, 100},
            {{{.weight = 1, .value = high}, {.weight = 1, .value = high}},
             2,
             2 * std::int64_t{high}}};
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
