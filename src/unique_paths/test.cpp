#include "solution.hpp"

#include <boost/ut.hpp>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <rapidcheck.h>
#include <tuple>
#include <vector>

namespace {
// Choose which of the moves go down, using exact stepwise division.
auto binomial(std::uint64_t n, std::uint64_t k) -> std::uint64_t {
    auto result = std::uint64_t{1};
    for (auto i = std::uint64_t{1}; i <= k; ++i) {
        result = result * (n - k + i) / i;
    }
    return result;
}
} // namespace

auto main() -> int try {
    using namespace boost::ut;

    "matches the binomial coefficient in either orientation"_test = [] -> void {
        expect(rc::check([] -> void {
            const auto rows = *rc::gen::inRange<std::size_t>(1, 18);
            const auto columns = *rc::gen::inRange<std::size_t>(1, 18);
            const auto expected = binomial(rows + columns - 2, rows - 1);
            RC_ASSERT(algos::unique_paths(rows, columns) == expected);
            RC_ASSERT(algos::unique_paths(columns, rows) == expected);
        }));
    };

    "single cells, thin grids, and the largest square"_test =
        [](const auto &example) -> void {
        const auto &[rows, columns, expected] = example;
        expect(that % algos::unique_paths(rows, columns) == expected);
    } | std::vector<std::tuple<std::size_t, std::size_t, std::uint64_t>>{
            {1, 1, 1},
            {3, 2, 3},
            {3, 4, 10},
            {3, 7, 28},
            {1, 1'000'000, 1},
            {1'000'000, 1, 1},
            {34, 34, 7'219'428'434'016'265'740U}};
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
