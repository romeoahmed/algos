#include "solution.hpp"

#include <boost/ut.hpp>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <rapidcheck.h>
#include <utility>
#include <vector>

namespace {
// Enumerate every walk by trying both step sizes from each reached stair.
auto enumerate_walks(std::size_t remaining) -> std::uint64_t {
    if (remaining == 0) {
        return 1;
    }
    const auto one = enumerate_walks(remaining - 1);
    return remaining < 2 ? one : one + enumerate_walks(remaining - 2);
}
} // namespace

auto main() -> int try {
    using namespace boost::ut;

    "matches exhaustive enumeration"_test = [] -> void {
        expect(rc::check([] -> void {
            const auto n = *rc::gen::inRange<std::size_t>(0, 26);
            RC_ASSERT(algos::climbing_stairs(n) == enumerate_walks(n));
        }));
    };

    "splits walks by their last step up to the uint64 limit"_test = [] -> void {
        expect(rc::check([] -> void {
            const auto n = *rc::gen::inRange<std::size_t>(2, 93);
            RC_ASSERT(algos::climbing_stairs(n) ==
                      algos::climbing_stairs(n - 1) +
                          algos::climbing_stairs(n - 2));
        }));
    };

    "no stairs, small counts, and the largest representable count"_test =
        [](const auto &example) -> void {
        const auto &[n, expected] = example;
        expect(that % algos::climbing_stairs(n) == expected);
    } | std::vector<std::pair<std::size_t, std::uint64_t>>{
            {0, 1}, {1, 1}, {2, 2},
            {4, 5}, {5, 8}, {92, 12'200'160'415'121'876'738U}};
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
