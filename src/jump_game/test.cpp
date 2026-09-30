#include "solution.hpp"

#include <algorithm>
#include <boost/ut.hpp>
#include <cstddef>
#include <exception>
#include <iostream>
#include <limits>
#include <rapidcheck.h>
#include <span>
#include <utility>
#include <vector>

namespace {
// Shortest paths in the forward graph, independent of greedy layering.
auto shortest_path(const std::vector<std::size_t> &values) -> std::size_t {
    auto distances = std::vector(values.size(), values.size());
    distances.front() = 0;
    for (auto i = std::size_t{0}; i < values.size(); ++i) {
        for (auto j = i + 1; j < values.size() && j - i <= values.at(i); ++j) {
            distances.at(j) = std::min(distances.at(j), distances.at(i) + 1);
        }
    }
    return distances.back();
}
} // namespace

auto main() -> int try {
    using namespace boost::ut;
    "matches shortest paths on reachable arrays"_test = [] -> void {
        expect(rc::check([] -> void {
            const auto size = *rc::gen::inRange<std::size_t>(1, 31);
            auto values = *rc::gen::container<std::vector<std::size_t>>(
                size, rc::gen::inRange<std::size_t>(0, 21));
            auto start = std::size_t{0};
            while (start + 1 < size) {
                const auto stop = *rc::gen::inRange(start + 1, size);
                values.at(start) = std::max(values.at(start), stop - start);
                start = stop;
            }
            const auto before = values;
            const auto expected = shortest_path(values);
            RC_ASSERT(algos::jump_game(values) == expected);
            RC_ASSERT(values == before);
        }));
    };
    "singleton, zeros, exact layers, and oversized jumps"_test =
        [](const auto &example) -> void {
        const auto &[values, expected] = example;
        expect(that % algos::jump_game(values) == expected);
    } | std::vector<std::pair<std::vector<std::size_t>, std::size_t>>{
            {{0}, 0},
            {{2, 3, 1, 1, 4}, 2},
            {{2, 3, 0, 1, 4}, 2},
            {{2, 3, 0, 0, 0}, 2},
            {std::vector<std::size_t>(80, 1), 79},
            {{std::numeric_limits<std::size_t>::max(), 0, 0}, 1},
            {{1, std::numeric_limits<std::size_t>::max(), 0}, 2}};
    "uses indices relative to the supplied subspan"_test = [] -> void {
        const auto values = std::vector<std::size_t>{5, 1, 1, 0, 0};
        expect(that % algos::jump_game(std::span{values}.subspan(1, 3)) == 2U);
    };
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
