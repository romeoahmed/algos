#include "solution.hpp"

#include <algorithm>
#include <boost/ut.hpp>
#include <cstddef>
#include <exception>
#include <iostream>
#include <ranges>
#include <rapidcheck.h>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

namespace {
// Try every subsequence of first and keep the longest one found in second.
auto exhaustive(const std::string &first, const std::string &second)
    -> std::size_t {
    auto best = std::size_t{0};
    for (auto mask = std::size_t{0}; mask < std::size_t{1} << first.size();
         ++mask) {
        auto matched = std::size_t{0};
        auto size = std::size_t{0};
        auto within = true;
        for (auto i = std::size_t{0}; i < first.size() && within; ++i) {
            if ((mask >> i & 1U) == 0) {
                continue;
            }
            const auto found = second.find(first.at(i), matched);
            within = found != std::string::npos;
            matched = found + 1;
            ++size;
        }
        if (within) {
            best = std::max(best, size);
        }
    }
    return best;
}

auto repeat(std::string_view piece, std::size_t count) -> std::string {
    return std::views::repeat(piece, count) | std::views::join |
           std::ranges::to<std::string>();
}
} // namespace

auto main() -> int try {
    using namespace boost::ut;

    "matches exhaustive search in either order"_test = [] -> void {
        expect(rc::check([] -> void {
            const auto text = [] -> std::string {
                const auto size = *rc::gen::inRange<std::size_t>(0, 10);
                return *rc::gen::container<std::string>(
                    size, rc::gen::element('a', 'b', 'c'));
            };
            const auto first = text();
            const auto second = text();
            const auto expected = exhaustive(first, second);
            RC_ASSERT(algos::longest_common(first, second) == expected);
            RC_ASSERT(algos::longest_common(second, first) == expected);
        }));
    };

    "empty, identical, disjoint, repeated, and long strings"_test =
        [](const auto &example) -> void {
        const auto &[first, second, expected] = example;
        expect(that % algos::longest_common(first, second) == expected);
    } | std::vector<std::tuple<std::string, std::string, std::size_t>>{
            {"", "", 0},
            {"", "abc", 0},
            {"abc", "abc", 3},
            {"abc", "def", 0},
            {"abcde", "ace", 3},
            {"ace", "abcde", 3},
            {"abcba", "abcbcba", 5},
            {repeat("ab", 150), repeat("ba", 100), 200}};
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
